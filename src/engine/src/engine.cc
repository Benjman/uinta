#include "uinta/engine.h"

#include <cassert>

#include "uinta/engine_state.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

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

    newFrame();
  }
}

Engine::Engine(Engine&& other) noexcept
    : status_(other.status_),
      state_(std::move(other.state_)),
      platform_(std::move(other.platform_)) {}

Engine& Engine::operator=(Engine&& other) noexcept {
  if (this != &other) {
    status_ = other.status_;
    state_ = std::move(other.state_);
    platform_ = std::move(other.platform_);
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
}

}  // namespace uinta
