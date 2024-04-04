#include "uinta/engine.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : frameManager_(platform), gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  setCallbacks();

  onViewportChange(platform_->window()->width(), platform_->window()->height());

  while (!state_.isClosing() && status_.ok()) {
    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    do {
      if (auto status = platform_->runtime(); status.ok()) {
        state_.addTicks(1);
        state_.addDelta(status.value() - state_.runtime());
      } else {
        status_ = status.status();
        return;
      }

      state_.newFrame(false);
    } while (state_.runtime() < frameManager_.nextFrame);

    newFrame();
  }
}

void Engine::newFrame() noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!status_.ok()) return;

  state_.newFrame(true);
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

void Engine::onViewportChange(u32 width, u32 height) noexcept {
  gl_->viewport(0, 0, width, height);
}

}  // namespace uinta
