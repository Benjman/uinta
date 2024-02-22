#include "uinta/engine.h"

#include <algorithm>
#include <cassert>

#include "absl/log/log.h"
#include "uinta/engine_signal.h"
#include "uinta/engine_state.h"
#include "uinta/input.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"
#include "uinta/scene.h"

namespace uinta {

void updateSceneViewport(Scene* scene, const ViewportSizeChange&) noexcept;

void preTick(Scene*, const EngineState&, const Input&) noexcept;
void tick(Scene*, const EngineState&, const Input&) noexcept;
void postTick(Scene*, const EngineState&, const Input&) noexcept;
void newFrame(Scene*, const EngineState&, const Input&) noexcept;
void preRender(Scene*, const EngineState&, const Input&) noexcept;
void render(Scene*, const EngineState&, const Input&) noexcept;
void postRender(Scene*, const EngineState&, const Input&) noexcept;

Engine::Engine(Platform* platform, Scene* scene, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform), scene_(scene) {
  assert(platform_ && "`Platform*` cannot be null.");
  assert(scene_ && "`Scene*` cannot be null.");

  Input input;
  setCallbacks(input);

  onViewportChange(platform_->window()->width(), platform_->window()->height());

  while (!state_.isClosing() && status_.ok()) {
    auto runtime = platform_->runtime();
    if (!runtime.ok()) {
      status_ = runtime.status();
      return;
    }
    state_.advance(runtime.value());

    input.reset();
    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    preTick(scene, state_, input);
    tick(scene, state_, input);
    postTick(scene, state_, input);
    state_.isNewFrame(false);

    newFrame(input);
    preRender(scene, state_, input);
    render(scene, state_, input);
    postRender(scene, state_, input);
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

void Engine::newFrame(const Input& input) noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!status_.ok()) return;
  state_.isNewFrame(true);
  components_.update(Component::Stage::NewFrame, state_);
  uinta::newFrame(scene_, state_, input);
}

void Engine::setCallbacks(Input& input) noexcept {
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

  if (status_ = platform_->registerInputHandlers(&input); !status_.ok()) {
    LOG(ERROR) << status_.message();
    return;
  }
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

void newFrame(Scene* scene, const EngineState& state,
              const Input& input) noexcept {
  scene->components().update(Component::Stage::NewFrame, state);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { newFrame(scene.get(), state, input); });
}

void preTick(Scene* scene, const EngineState& state,
             const Input& input) noexcept {
  scene->components().update(Component::Stage::PreTick, state);
  scene->preTick(state, input);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { preTick(scene.get(), state, input); });
}

void tick(Scene* scene, const EngineState& state, const Input& input) noexcept {
  scene->components().update(Component::Stage::Tick, state);
  scene->tick(state, input);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { tick(scene.get(), state, input); });
}

void postTick(Scene* scene, const EngineState& state,
              const Input& input) noexcept {
  scene->components().update(Component::Stage::PostTick, state);
  scene->postTick(state, input);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { postTick(scene.get(), state, input); });
}

void preRender(Scene* scene, const EngineState& state,
               const Input& input) noexcept {
  scene->components().update(Component::Stage::PreRender, state);
  scene->preRender(state, input);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { preRender(scene.get(), state, input); });
}

void render(Scene* scene, const EngineState& state,
            const Input& input) noexcept {
  scene->components().update(Component::Stage::Render, state);
  scene->render(state, input);
  std::for_each(Scene::RenderOrder.begin(), Scene::RenderOrder.end(),
                [&scene, &state, &input](auto layer) {
                  std::for_each(scene->scenes().begin(), scene->scenes().end(),
                                [layer, &state, &input](auto& scene) {
                                  if (scene->layer() == layer) {
                                    render(scene.get(), state, input);
                                  }
                                });
                });
}

void postRender(Scene* scene, const EngineState& state,
                const Input& input) noexcept {
  scene->components().update(Component::Stage::PostRender, state);
  scene->postRender(state, input);
  std::for_each(
      scene->scenes().begin(), scene->scenes().end(),
      [&state, &input](auto& scene) { postRender(scene.get(), state, input); });
}

}  // namespace uinta
