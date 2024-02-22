#include "uinta/engine/engine.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "uinta/scene/scene_events.h"

namespace uinta {

Engine::Engine(Params params) noexcept
    : components_(this),
      frame_(params.platform->primaryMonitor().value_or(nullptr)),
      appConfig_(params.appConfig),
      fileSystem_(params.fileSystem),
      gl_(params.gl),
      platform_(params.platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  platform_->engine(this);

  platform_->addListener<PlatformEvent::OnCloseRequest>(
      [this](const auto&) { state_.isClosing(true); });

  platform_->addListener<PlatformEvent::OnError>([this](const auto& event) {
    auto msg = absl::StrFormat("%i: %s", event.code, event.description);
    setStatusError(InternalError(msg));
  });

  platform_->addListener<PlatformEvent::OnDebugMessage>([](const auto& event) {
    std::string message(event.message, event.length);
    message = absl::StrCat(message, "\n\tID: ", event.id);
    message = absl::StrCat(message, "\n\tSeverity: ",
                           OpenGLApi::GetSeverityString(event.severity));
    message = absl::StrCat(message, "\n\tSource: ", event.source, "\t",
                           OpenGLApi::GetSourceString(event.source));
    message = absl::StrCat(message, "\n\tType: ", event.type, "\t",
                           OpenGLApi::GetTypeString(event.type));

    switch (event.severity) {
      case GL_DEBUG_SEVERITY_NOTIFICATION:
        LOG(INFO) << message;
        break;
      case GL_DEBUG_SEVERITY_LOW:
        LOG(WARNING) << message;
        break;
      case GL_DEBUG_SEVERITY_MEDIUM:
        LOG(ERROR) << message;
        break;
      default:
        LOG(FATAL) << message;
        break;
    }
  });

  platform_->addListener<PlatformEvent::OnViewportSizeChange>(
      [this](const auto&) {
        auto width = platform_->window()->width();
        auto height = platform_->window()->height();
        gl_->viewport(0, 0, width, height);
        dispatchers_.dispatch<EngineEvent::ViewportSizeChange>(
            ViewportSizeChange(width, height));
        LOG(INFO) << absl::StrFormat("Event: Viewport size change (%u, %u)",
                                     width, height);
      });

  gl_->clearColor(1.0, 1.0, 1.0, 1.0);

  platform_->addListener<PlatformEvent::OnMonitorChange>(
      [this](const auto& event) { frame_ = FrameManager(event.monitor); });

  if (auto status = platform_->registerInputHandlers(&input_); !status.ok()) {
    setStatusError(status);
    return;
  }
}

void Engine::run() noexcept {
  Scene* scene = nullptr;

  while (!state_.isClosing() && status_.ok()) {
    if (!scene && !sceneQueue_.empty()) {
      if (scene = sceneQueue_.front().get(); scene) {
        registerSceneListeners(scene);
        updateRenderOrder();
      }
    }

    if (scene) {
      if (!scene->status().ok()) {
        setStatusError(scene->status());
        break;
      } else if (scene->isComplete()) {
        sceneQueue_.pop();
        scene = nullptr;
        continue;
      }

      scene->removeStaleScenes();
    }

    input_.reset();
    if (auto status = platform_->pollEvents(); !status.ok()) {
      setStatusError(status);
      break;
    }

    do {
      state_.updateRuntime(getRuntime());

      advance<EngineStage::PreTick>(scene);
      advance<EngineStage::Tick>(scene);
      advance<EngineStage::PostTick>(scene);

      state_.addTick();

      dispatchers_.dispatch<EngineEvent::TickComplete>(
          TickComplete(&state_, getRuntime()));
    } while (state_.runtime() < frame_.next);

    state_.updateRuntime(getRuntime());

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    advance<EngineStage::PreRender>(scene);
    advance<EngineStage::Render>(scene);
    advance<EngineStage::PostRender>(scene);

    if (auto status = platform_->swapBuffers(); !status.ok()) {
      setStatusError(status);
      break;
    }

    state_.addFrame();

    frame_.next = state_.runtime();
    if (!state_.isFixedTickRate()) {
      frame_.next += frame_.frequency;
    }

    dispatchers_.dispatch<EngineEvent::RenderComplete>(
        RenderComplete(&state_, getRuntime()));
  }
}

void Engine::updateRenderOrder() noexcept {
  struct Comparator final {
    bool operator()(const Scene* a, const Scene* b) const noexcept {
      return a->layer() > b->layer();
    }
  };

  std::priority_queue<Scene*, std::vector<Scene*>, Comparator> queue;

  if (!sceneQueue_.empty()) {
    auto* front = sceneQueue_.front().get();
    queue.push(front);
    std::for_each(front->children().begin(), front->children().end(),
                  [&queue](auto& scene) {
                    queue.push(scene.get());
                    std::for_each(
                        scene->children().begin(), scene->children().end(),
                        [&queue](auto& child) { queue.push(child.get()); });
                  });
  }

  renderOrder_.clear();
  renderOrder_.reserve(queue.size());

  while (!queue.empty()) {
    renderOrder_.push_back(queue.top());
    queue.pop();
  }

  LOG(INFO) << "Render order updated.";
}

void Engine::registerSceneListeners(Scene* scene) noexcept {
  assert(scene);

  scene->dispatchers()->template addListener<SceneEvent::SceneAdded>(
      [this](const auto&) { updateRenderOrder(); });
  scene->dispatchers()->template addListener<SceneEvent::SceneRemoved>(
      [this](const auto&) { updateRenderOrder(); });
  scene->dispatchers()->template addListener<SceneEvent::LayerChange>(
      [this](const auto&) { updateRenderOrder(); });

  for (auto& child : scene->children()) {
    registerSceneListeners(child.get());
  }
}

void Engine::preTick(Scene* scene) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->preTick(state_.delta());
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this](auto& scene) { preTick(scene.get()); });
  }
}

void Engine::tick(Scene* scene) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->tick(state_.delta());
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this](auto& scene) { tick(scene.get()); });
  }
}

void Engine::postTick(Scene* scene) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->postTick(state_.delta());
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this](auto& scene) { postTick(scene.get()); });
  }
}

void Engine::preRender(Scene* scene) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->preRender(state_.delta());
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this](auto& scene) { preRender(scene.get()); });
  }
}

void Engine::render(Scene* scene) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->render(state_.delta());
  }
}

void Engine::postRender(Scene* scene) noexcept {
  if (scene->isRendering()) {
    scene->postRender(state_.delta());
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this](auto& scene) { postRender(scene.get()); });
  }
}

}  // namespace uinta
