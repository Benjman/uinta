#include "uinta/engine/engine.h"

#include <cassert>
#include <string>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"

namespace uinta {

Engine::Engine(Params params) noexcept
    : fileSystem_(params.fileSystem),
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

  gl_->clearColor(0.1, 0.1, 0.1, 1.0);
}

void Engine::run() noexcept {
  while (!state_.isClosing() && status_.ok()) {
    if (auto status = platform_->pollEvents(); !status.ok()) {
      setStatusError(status);
      break;
    }

    state_.updateRuntime(getRuntime());

    advance<EngineStage::PreTick>();
    advance<EngineStage::Tick>();
    advance<EngineStage::PostTick>();

    state_.addTick();

    dispatchers_.dispatch<EngineEvent::TickComplete>(
        TickComplete(&state_, getRuntime()));

    state_.updateRuntime(getRuntime());

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();

    if (auto status = platform_->swapBuffers(); !status.ok()) {
      setStatusError(status);
      break;
    }

    state_.addFrame();

    dispatchers_.dispatch<EngineEvent::RenderComplete>(
        RenderComplete(&state_, getRuntime()));
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {}

void Engine::postRender() noexcept {}

}  // namespace uinta
