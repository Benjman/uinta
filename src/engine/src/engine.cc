#include "uinta/engine.h"

#include <algorithm>
#include <cassert>

#include "uinta/engine_signal.h"
#include "uinta/engine_state.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"
#include "uinta/scene.h"

namespace uinta {

void updateSceneViewport(Scene* scene, const ViewportSizeChange&) noexcept;

void preTick(Scene*, const EngineState&) noexcept;
void tick(Scene*, const EngineState&) noexcept;
void postTick(Scene*, const EngineState&) noexcept;
void newFrame(Scene*, const EngineState&) noexcept;
void preRender(Scene*, const EngineState&) noexcept;
void render(Scene*, const EngineState&) noexcept;
void postRender(Scene*, const EngineState&) noexcept;

Engine::Engine(Platform* platform, Scene* scene, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform), scene_(scene) {
  assert(platform_ && "`Platform*` cannot be null.");
  assert(scene_ && "`Scene*` cannot be null.");

  setCallbacks();

  onViewportChange(platform_->window()->width(), platform_->window()->height());

  while (!state_.isClosing() && status_.ok()) {
    auto runtime = platform_->runtime();
    if (!runtime.ok()) {
      status_ = runtime.status();
      return;
    }
    state_.advance(runtime.value());

    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    preTick(scene, state_);
    tick(scene, state_);
    postTick(scene, state_);
    state_.isNewFrame(false);

    newFrame();
    preRender(scene, state_);
    render(scene, state_);
    postRender(scene, state_);
  }
}

Engine::Engine(Engine&& other) noexcept
    : status_(other.status_),
      state_(std::move(other.state_)),
      components_(std::move(other.components_)),
      platform_(std::move(other.platform_)),
      scene_(other.scene_) {}

Engine& Engine::operator=(Engine&& other) noexcept {
  if (this != &other) {
    status_ = other.status_;
    state_ = std::move(other.state_);
    components_ = std::move(other.components_);
    platform_ = std::move(other.platform_);
    scene_ = other.scene_;
  }
  return *this;
}

void Engine::newFrame() noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!status_.ok()) return;
  state_.isNewFrame(true);
  components_.update(Component::Stage::NewFrame, state_);
  uinta::newFrame(scene_, state_);
}

void Engine::setCallbacks() noexcept {
  platform_->onCloseRequest([&](void*) { state_.setClosing(); });

  platform_->onError([&](i32 code, const char* description) {
    status_ = InternalError(StrFormat("%i: %s", code, description));
  });

  platform_->onDebugMessage([&](GLenum /* source */, GLenum /* type */,
                                GLuint /* id */, GLenum severity,
                                GLsizei /* length */, const char* message,
                                const void* /* userParam */) {
    status_ = InternalError(StrFormat("[%i]: %s", severity, message));
  });

  platform_->onFramebufferSize(
      [&](u32 width, u32 height) { onViewportChange(width, height); });
}

void Engine::onViewportChange(u32 width, u32 height) noexcept {
  gl_->viewport(0, 0, width, height);
  updateSceneViewport(scene_, {width, height});
}

void updateSceneViewport(Scene* scene,
                         const ViewportSizeChange& event) noexcept {
  scene->onViewportSizeChange(event);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&event](auto& scene) { updateSceneViewport(scene.get(), event); });
}

void newFrame(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::NewFrame, state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { newFrame(scene.get(), state); });
}

void preTick(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::PreTick, state);
  scene->preTick(state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { preTick(scene.get(), state); });
}

void tick(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::Tick, state);
  scene->tick(state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { tick(scene.get(), state); });
}

void postTick(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::PostTick, state);
  scene->postTick(state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { postTick(scene.get(), state); });
}

void preRender(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::PreRender, state);
  scene->preRender(state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { preRender(scene.get(), state); });
}

void render(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::Render, state);
  scene->render(state);
  std::for_each(Scene::RenderOrder.begin(), Scene::RenderOrder.end(),
                [&scene, &state](auto layer) {
                  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                                [layer, &state](auto& scene) {
                                  if (scene->layer() == layer) {
                                    render(scene.get(), state);
                                  }
                                });
                });
}

void postRender(Scene* scene, const EngineState& state) noexcept {
  scene->components().update(Component::Stage::PostRender, state);
  scene->postRender(state);
  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                [&state](auto& scene) { postRender(scene.get(), state); });
}

}  // namespace uinta
