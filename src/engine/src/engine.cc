#include "uinta/engine.h"

#include <algorithm>
#include <cassert>

#include "absl/log/log.h"
#include "glm/ext/vector_uint2.hpp"
#include "uinta/engine_stage.h"
#include "uinta/engine_state.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"
#include "uinta/scene.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

template <EngineStage S>
void advance(Scene*, EngineState*) noexcept;

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : frameManager_(platform), gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

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

void Engine::runScene(std::unique_ptr<Scene> scene) noexcept {
  assert(scene && "`Scene*` cannot be null.");

  f32 previousRuntime = 0;
  if (auto status = platform_->runtime(); status.ok()) {
    previousRuntime = status.value();
  } else {
    status_ = status.status();
    return;
  }

  while (!state_.isClosing() && status_.ok()) {
    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    if (hasFramebufferResized()) {
      glm::uvec2 size(platform_->window()->width(),
                      platform_->window()->height());
      gl_->viewport(0, 0, size.x, size.y);
      updateSceneViewport(scene.get(), {size.x, size.y});
      flags_ &= ~FramebufferResizedMask;
    }

    do {
      if (auto status = platform_->runtime(); status.ok()) {
        auto delta = status.value() - previousRuntime;
        previousRuntime = status.value();
        state_.addTicks(1);
        state_.addDelta(delta);
      } else {
        status_ = status.status();
        return;
      }

      advance<EngineStage::PreTick>(scene.get(), &state_);
      advance<EngineStage::Tick>(scene.get(), &state_);
      advance<EngineStage::PostTick>(scene.get(), &state_);

      if (state_.isNewFrame()) state_.newFrame(false);
    } while (state_.runtime() < frameManager_.nextFrame);

    if (auto status = platform_->runtime(); status.ok()) {
      auto delta = status.value() - previousRuntime;
      previousRuntime = status.value();
      state_.addDelta(delta);
    } else {
      status_ = status.status();
      return;
    }

    newFrame(scene.get());

    advance<EngineStage::PreRender>(scene.get(), &state_);
    advance<EngineStage::Render>(scene.get(), &state_);
    advance<EngineStage::PostRender>(scene.get(), &state_);
  }
}

void cleanScene(Scene*) noexcept;
void newFrame(Scene*, const EngineState&) noexcept;

void Engine::newFrame(Scene* scene) noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!status_.ok()) return;

  state_.newFrame(true);
  cleanScene(scene);
  uinta::newFrame(scene, state_);
  state_.resetFrameDelta();
  frameManager_.nextFrame = state_.runtime() + frameManager_.nextFrameAdvance;
}

void Engine::onViewportChange(u32 width, u32 height) noexcept {
  i32 w, h;
  platform_->getAndUpdateWindowSize(&w, &h);
  assert(
      width == platform_->window()->width() &&
      StrFormat("Unexpected width:\n\tIncoming value: %f\n\tPlatform value: %f",
                width, platform_->window()->width())
          .c_str());
  assert(height == platform_->window()->height() &&
         StrFormat(
             "Unexpected height:\n\tIncoming value: %f\n\tPlatform value: %f",
             height, platform_->window()->height())
             .c_str());
  flags_ |= FramebufferResizedMask;
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
  // FIXME: The current rendering order of scenes is incorrect. Each scene
  // manages its own collection of sub-scenes, each with a specific rendering
  // order. To resolve this issue, we need to implement a methodical sorting
  // mechanism within the scene tree. Then render in that order.
  //
  // Although bugged, this is working.
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

}  // namespace uinta
