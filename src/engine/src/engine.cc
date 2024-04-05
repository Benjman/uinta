#include "uinta/engine/engine.h"

#include <algorithm>
#include <cassert>

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : components_(this), frame_(platform), gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  platform_->addListener<PlatformEvent::OnCloseRequest>(
      [&](const auto&) { state_.isClosing(true); });

  platform_->addListener<PlatformEvent::OnError>([&](const auto& event) {
    status_ = InternalError(StrFormat("%i: %s", event.code, event.description));
  });

  platform_->addListener<PlatformEvent::OnDebugMessage>([&](const auto& event) {
    std::string message(event.message, event.length);
    message = StrCat(message, "\n\tID: ", event.id);
    message = StrCat(message,
                     "\n\tSeverity: ", gl_->getSeverityString(event.severity));
    message = StrCat(message, "\n\tSource: ", event.source, "\t",
                     gl_->getSourceString(event.source));
    message = StrCat(message, "\n\tType: ", event.type, "\t",
                     gl_->getTypeString(event.type));

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

  platform_->addListener<PlatformEvent::OnViewportSizeChange>([&](const auto&) {
    auto width = platform_->window()->width();
    auto height = platform_->window()->height();
    gl_->viewport(0, 0, width, height);
    dispatch<EngineEvent::ViewportSizeChange>(
        ViewportSizeChange(width, height));
    LOG(INFO) << StrFormat("Event: Viewport size change (%u, %u)", width,
                           height);
  });

  platform_->addListener<PlatformEvent::OnMonitorChange>(
      [&](const auto& event) { frame_.onMonitorChange(event.monitor); });

  gl_->clearColor(0.1, 0.1, 0.1, 1.0);

  if (status_ = platform_->registerInputHandlers(&state_.input());
      !status_.ok()) {
    LOG(ERROR) << status_.message();
    return;
  }

  state_.update(getRuntime(), 0);
}

void Engine::run() noexcept {
  state_.update(getRuntime(), 0);

  Scene* scene = nullptr;
  time_t start;
  time_t runtime;

  while (!state_.isClosing() && status_.ok()) {
    if (!scene && !sceneQueue_.empty()) scene = sceneQueue_.front().get();

    if (status_ = platform_->pollEvents(); !status_.ok()) {
      LOG(ERROR) << "`Platform::pollEvents()` failed: " << status_.message();
      break;
    }

    do {
      runtime = getRuntime();
      start = runtime;
      state_.isNewFrame(runtime >= frame_.nextFrame);
      state_.update(runtime, 1);

      components_.update<EngineStage::PreTick>(state_);
      systems_.update<EngineStage::PreTick>(state_);
      advance<EngineStage::PreTick>(scene);

      components_.update<EngineStage::Tick>(state_);
      systems_.update<EngineStage::Tick>(state_);
      advance<EngineStage::Tick>(scene);

      components_.update<EngineStage::PostTick>(state_);
      systems_.update<EngineStage::PostTick>(state_);
      advance<EngineStage::PostTick>(scene);

      runtime = getRuntime();
      dispatch<EngineEvent::TickComplete>(
          TickComplete(&state_, runtime - start));
    } while (!state_.isNewFrame());

    components_.update<ComponentType::NewFrame>(state_);
    systems_.update<SystemType::NewFrame>(state_);
    if (scene) {
      scene->updateComponents<ComponentType::NewFrame>(state_);
      scene->updateSystems<SystemType::NewFrame>(state_);
    }

    runtime = getRuntime();
    start = runtime;
    frame_.nextFrame =
        runtime + (flags_.isFixedTickRate() ? 0 : frame_.nextFrameAdvance);

    components_.update<EngineStage::PreRender>(state_);
    systems_.update<EngineStage::PreRender>(state_);
    advance<EngineStage::PreRender>(scene);

    components_.update<EngineStage::Render>(state_);
    systems_.update<EngineStage::Render>(state_);
    advance<EngineStage::Render>(scene);

    components_.update<EngineStage::PostRender>(state_);
    systems_.update<EngineStage::PostRender>(state_);
    advance<EngineStage::PostRender>(scene);

    state_.isNewFrame(false);

    runtime = getRuntime();
    dispatch<EngineEvent::RenderComplete>(
        RenderComplete(&state_, runtime - start));

    if (status_ = platform_->swapBuffers(); !status_.ok()) {
      LOG(ERROR) << "`Platform::swapBuffers()` failed: " << status_.message();
      return;
    }

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    state_.input().reset();

    if (scene) {
      scene->removeStaleScenes();
      if (!scene || !scene->isRunning()) {
        if (scene && scene->isError()) {
          LOG(ERROR) << StrFormat("`%s` produced an error.", scene->name());
          status_ = CancelledError();
        }
        sceneQueue_.pop();
        scene = nullptr;
      }
    }
  }
}

void Engine::preTick(Scene* scene) noexcept {
  if (scene && scene->isTicking()) {
    scene->preTick(state_);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [&](auto& scene) { preTick(scene.get()); });
  }
}

void Engine::tick(Scene* scene) noexcept {
  if (scene && scene->isTicking()) {
    scene->tick(state_);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [&](auto& scene) { tick(scene.get()); });
  }
}

void Engine::postTick(Scene* scene) noexcept {
  if (scene && scene->isTicking()) {
    scene->postTick(state_);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [&](auto& scene) { postTick(scene.get()); });
  }
}

void Engine::preRender(Scene* scene) noexcept {
  if (scene && scene->isRendering()) {
    scene->preRender(state_);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [&](auto& scene) { preRender(scene.get()); });
  }
}

// FIXME: The current rendering order of scenes is incorrect. Each scene
// manages its own collection of sub-scenes, each with a specific rendering
// order. We need to implement a priority queue which we populate each new frame
// with the appropriate rendering order. Although incorrect, it is working.
void Engine::render(Scene* scene) noexcept {
  if (scene && scene->isRendering()) {
    scene->render(state_);
    std::for_each(Scene::RenderOrder.begin(), Scene::RenderOrder.end(),
                  [&](auto layer) {
                    std::for_each(scene->children().begin(),
                                  scene->children().end(), [&](auto& scene) {
                                    if (scene->layer() == layer)
                                      render(scene.get());
                                  });
                  });
  }
}

void Engine::postRender(Scene* scene) noexcept {
  if (scene && scene->isRendering()) {
    scene->postRender(state_);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [&](auto& scene) { postRender(scene.get()); });
  }
}

}  // namespace uinta
