#include "uinta/engine/engine.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform) {
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
    LOG(INFO) << StrFormat("Event: Viewport size change (%u, %u)", width,
                           height);
  });

  gl_->clearColor(0.1, 0.1, 0.1, 1.0);

  state_.update(getRuntime(), 0);
}

void Engine::run() noexcept {
  state_.update(getRuntime(), 0);

  time_t runtime;

  while (!state_.isClosing() && status_.ok()) {
    if (status_ = platform_->pollEvents(); !status_.ok()) {
      LOG(ERROR) << "`Platform::pollEvents()` failed: " << status_.message();
      break;
    }

    do {
      runtime = getRuntime();
      state_.isNewFrame(true);
      state_.update(runtime, 1);
      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();
      runtime = getRuntime();
    } while (!state_.isNewFrame());

    runtime = getRuntime();

    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();

    state_.isNewFrame(false);
    runtime = getRuntime();

    if (status_ = platform_->swapBuffers(); !status_.ok()) {
      LOG(ERROR) << "`Platform::swapBuffers()` failed: " << status_.message();
      return;
    }

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {}

void Engine::postRender() noexcept {}

}  // namespace uinta
