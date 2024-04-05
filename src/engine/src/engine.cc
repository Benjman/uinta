#include "uinta/engine.h"

#include <algorithm>
#include <cassert>

#include "absl/log/log.h"
#include "uinta/engine_stage.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"
#include "uinta/scene.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

template <EngineStage S>
void advance(Scene*, EngineState*) noexcept;

Engine::Engine(Platform* platform, Scene* scene, const OpenGLApi* gl) noexcept
    : frameManager_(platform), gl_(gl), platform_(platform), scene_(scene) {
  assert(platform_ && "`Platform*` cannot be null.");
  assert(scene_ && "`Scene*` cannot be null.");
  scene_->engine(this);

  setCallbacks();

  onViewportChange(platform_->window()->width(), platform_->window()->height());

  auto runtime = platform_->runtime();
  status_ = runtime.status();
  const auto startRuntime = status_.ok() ? runtime.value() : 0;

  while (!state_.isClosing() && status_.ok()) {
    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    do {
      if (runtime = platform_->runtime(); !runtime.ok()) {
        status_ = runtime.status();
        return;
      }

      state_.advance(runtime.value() - startRuntime);

      advance<EngineStage::PreTick>(scene_, &state_);
      advance<EngineStage::Tick>(scene_, &state_);
      advance<EngineStage::PostTick>(scene_, &state_);

      state_.newFrame(false);
    } while (state_.runtime() < frameManager_.nextFrame);

    newFrame();

    advance<EngineStage::PreRender>(scene_, &state_);
    advance<EngineStage::Render>(scene_, &state_);
    advance<EngineStage::PostRender>(scene_, &state_);
  }
}

void cleanScene(Scene*) noexcept;
void newFrame(Scene*, const EngineState&) noexcept;

void Engine::newFrame() noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!status_.ok()) return;

  state_.newFrame(true);
  cleanScene(scene_);
  uinta::newFrame(scene_, state_);
  state_.resetFrameDelta();
  frameManager_.nextFrame = state_.runtime() + frameManager_.nextFrameAdvance;
}

void Engine::setCallbacks() noexcept {
  platform_->onCloseRequest([&](void*) { state_.setClosing(); });

  platform_->onError([&](i32 code, const char* description) {
    status_ = InternalError(StrFormat("%i: %s", code, description));
  });

  platform_->onDebugMessage([&](GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const char* message) {
    auto fmtmsg = StrFormat(
        "%s\n\tID: %i\n\tSeverity: %s\n\tSource: %i\t%s\n\tType:   %i\t%s",
        std::string(message, length), id, gl_->getSeverityString(severity),
        source, gl_->getSourceString(source), type, gl_->getTypeString(type));
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
      LOG(INFO) << fmtmsg;
    else if (severity == GL_DEBUG_SEVERITY_LOW)
      LOG(WARNING) << fmtmsg;
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
      LOG(ERROR) << fmtmsg;
    else
      LOG(FATAL) << fmtmsg;
  });

  platform_->onFramebufferSize(
      [&](u32 width, u32 height) { onViewportChange(width, height); });
}

void updateSceneViewport(Scene*, const ViewportSizeChange&) noexcept;

void Engine::onViewportChange(u32 width, u32 height) noexcept {
  gl_->viewport(0, 0, width, height);
  updateSceneViewport(scene_, {width, height});
}

void postRender(Scene*, const EngineState&) noexcept;
void postTick(Scene*, const EngineState&) noexcept;
void preRender(Scene*, const EngineState&) noexcept;
void preTick(Scene*, const EngineState&) noexcept;
void render(Scene*, const EngineState&) noexcept;
void tick(Scene*, const EngineState&) noexcept;

template <EngineStage S>
void advance(Scene* scene, EngineState* state) noexcept {
  state->stage(S);
  []() {
    if constexpr (S == EngineStage::PreTick)
      return preTick;
    else if constexpr (S == EngineStage::Tick)
      return tick;
    else if constexpr (S == EngineStage::PostTick)
      return postTick;
    else if constexpr (S == EngineStage::PreRender)
      return preRender;
    else if constexpr (S == EngineStage::Render)
      return render;
    else if constexpr (S == EngineStage::PostRender)
      return postRender;
  }()(scene, *state);
}

void updateSceneViewport(Scene* scene,
                         const ViewportSizeChange& event) noexcept {
  scene->onViewportSizeChange(event);
  std::for_each(
      scene->children().stack().begin(), scene->children().stack().end(),
      [&event](auto& scene) { updateSceneViewport(scene.get(), event); });
}

void newFrame(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::NewFrame>(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { newFrame(scene.get(), state); });
}

void preTick(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::PreTick>(state);
  scene->preTick(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { preTick(scene.get(), state); });
}

void tick(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::Tick>(state);
  scene->tick(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { tick(scene.get(), state); });
}

void postTick(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::PostTick>(state);
  scene->postTick(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { postTick(scene.get(), state); });
}

void preRender(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::PreRender>(state);
  scene->preRender(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { preRender(scene.get(), state); });
}

void render(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::Render>(state);
  scene->render(state);
  std::for_each(Scene::RenderOrder.begin(), Scene::RenderOrder.end(),
                [&scene, &state](auto layer) {
                  std::for_each(scene->children().stack().begin(),
                                scene->children().stack().end(),
                                [layer, &state](auto& scene) {
                                  if (scene->layer() == layer)
                                    render(scene.get(), state);
                                });
                });
}

void postRender(Scene* scene, const EngineState& state) noexcept {
  scene->updateSystems<EngineStage::PostRender>(state);
  scene->postRender(state);
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [&state](auto& scene) { postRender(scene.get(), state); });
}

void cleanScene(Scene* scene) noexcept {
  scene->removeStaleScenes();
  std::for_each(scene->children().stack().begin(),
                scene->children().stack().end(),
                [](auto& scene) { cleanScene(scene.get()); });
}

Engine::FrameManager::FrameManager(const Platform* platform)
    : nextFrameAdvance(1.0f /
                       static_cast<f32>(platform->primaryMonitor()->hz())) {}

}  // namespace uinta
