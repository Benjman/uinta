#include "uinta/engine/engine.h"

#include <absl/log/log.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>

#include <algorithm>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

#include "uinta/app_config.h"
#include "uinta/input/input_frame_gurad.h"
#include "uinta/input/input_system.h"
#include "uinta/scene/scene_events.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

Engine::Engine(Params params) noexcept
    : inputSystem_(this),
      viewport(this, params.appConfig),
      frame_(params.platform->primaryMonitor().value_or(nullptr)),
      gl_(params.gl),
      platform_(params.platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  registerService<AppConfig>(params.appConfig);
  registerService<ViewportManager>(&viewport);
  registerService<InputSystem>(&inputSystem_);

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
        gl_->viewport(0, 0, static_cast<i32>(width), static_cast<i32>(height));
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
    if ((scene == nullptr) && !sceneQueue_.empty()) {
      if (scene = sceneQueue_.front().get(); scene != nullptr) {
        registerSceneListeners(scene);
        updateRenderOrder();
      }
    }

    if (scene != nullptr) {
      if (!scene->status().ok()) {
        setStatusError(scene->status());
        break;
      }
      if (scene->isComplete()) {
        sceneQueue_.pop();
        scene = nullptr;
        continue;
      }

      scene->removeStaleScenes();
    }

    InputFrameGuard inputGuard(this);
    if (!status_.ok()) {
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

void Engine::preTick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->preTick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { preTick(scene.get(), delta); });
  }
}

void Engine::tick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->tick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { tick(scene.get(), delta); });
  }
}

void Engine::postTick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->postTick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { postTick(scene.get(), delta); });
  }
}

void Engine::preRender(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->preRender(delta);
    std::for_each(
        scene->children().begin(), scene->children().end(),
        [this, delta](auto& scene) { preRender(scene.get(), delta); });
  }
}

void Engine::render(  // NOLINT(readability-convert-member-functions-to-static)
    Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->render(delta);
  }
}

void Engine::postRender(Scene* scene, time_t delta) noexcept {
  if (scene->isRendering()) {
    scene->postRender(delta);
    std::for_each(
        scene->children().begin(), scene->children().end(),
        [this, delta](auto& scene) { postRender(scene.get(), delta); });
  }
}

}  // namespace uinta
