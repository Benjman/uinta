#include "uinta/engine/engine.h"

#include <absl/log/log.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <string>

#include "uinta/args.h"
#include "uinta/gl.h"

namespace uinta {

namespace _ {

void registerService(Engine* engine, std::type_index type, void* service, bool isConst) noexcept {
  engine->registerService(type, service, isConst);
}

void unregisterService(Engine* engine, std::type_index type) noexcept { engine->unregisterService(type); }

}  // namespace _

Engine::Engine(const EngineDependencies& deps) noexcept {
  assert(deps.args && "Engine::Engine(): ArgsProcessor cannot be null!");
  registerService<const ArgsProcessor>(deps.args);
  assert(deps.gl && "Engine::Engine(): OpenGLApi cannot be null!");
  registerService<const OpenGLApi>(deps.gl);
  assert(deps.platform && "Engine::Engine(): Platform cannot be null!");
  registerService<Platform>(deps.platform);

  auto* platform = service<Platform>();
  platform->engine(this);
  platform->addListener<PlatformEvent::OnCloseRequest>([this](const auto&) { state_.isClosing(true); });
  platform->addListener<PlatformEvent::OnError>(
      [](const auto& event) { LOG(FATAL) << absl::StrFormat("%i: %s", event.code, event.description); });

  platform->addListener<PlatformEvent::OnDebugMessage>([](const auto& event) {
    std::string message(event.message, event.length);
    message = absl::StrCat(message, "\n\tID: ", event.id);
    message = absl::StrCat(message, "\n\tSeverity: ", OpenGLApi::GetSeverityString(event.severity));
    message = absl::StrCat(message, "\n\tSource: ", event.source, "\t", OpenGLApi::GetSourceString(event.source));
    message = absl::StrCat(message, "\n\tType: ", event.type, "\t", OpenGLApi::GetTypeString(event.type));
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

  platform->addListener<PlatformEvent::OnViewportSizeChange>([this, platform](const auto&) {
    auto width = platform->window()->width();
    auto height = platform->window()->height();
    const auto* gl = service<const OpenGLApi>();
    gl->viewport(0, 0, static_cast<i32>(width), static_cast<i32>(height));
    dispatchers_.dispatch<EngineEvent::ViewportSizeChange>(ViewportSizeChange(width, height));
    LOG(INFO) << absl::StrFormat("Event: Viewport size change (%u, %u)", width, height);
  });
}

void Engine::run() noexcept {
  const auto* platform = service<Platform>();
  const auto* gl = service<const OpenGLApi>();
  time_t startTime = platform->runtime();
  while (!state_.isClosing()) {
    if (auto status = platform->pollEvents(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    state_.updateRuntime(platform->runtime() - startTime);
    advance<EngineStage::PreTick>();
    advance<EngineStage::Tick>();
    advance<EngineStage::PostTick>();
    state_.addTick();
    // dispatchers_.dispatch<EngineEvent::TickComplete>(TickComplete(&state_, runtime())); // TODO: delete me
    state_.updateRuntime(platform->runtime() - startTime);
    gl->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();
    if (auto status = platform->swapBuffers(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    state_.addFrame();
    // dispatchers_.dispatch<EngineEvent::RenderComplete>(RenderComplete(&state_, runtime())); // TODO: delete me
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {}

void Engine::postRender() noexcept {}

}  // namespace uinta
