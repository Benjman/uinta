#include "uinta/engine/engine.h"

#include <absl/log/log.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <string>

#include "uinta/app_config.h"
#include "uinta/args.h"
#include "uinta/gl.h"
#include "uinta/localization/locale.h"
#include "uinta/localization/localization_system.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

namespace {

Locale resolveLocale(const ArgsProcessor* args) noexcept {
  assert(args && "Engine::Engine(): ArgsProcessor cannot be null!");
  if (auto val = args->getValue(ArgsProcessor::Locale)) {
    return toLocale(*val);
  }
  return Locale::EnUs;
}

time_t frameInterval(const Monitor* monitor) noexcept {
  if (monitor == nullptr || monitor->hz() == 0) {
    LOG(WARNING) << "Monitor refresh rate unavailable, defaulting frame interval to 60hz";
    return 1.0 / 60;
  }
  return 1.0 / monitor->hz();
}

}  // namespace

namespace _ {

void registerService(Engine* engine, std::type_index type, void* service, bool isConst) noexcept {
  engine->registerService(type, service, isConst);
}

void unregisterService(Engine* engine, std::type_index type) noexcept { engine->unregisterService(type); }

}  // namespace _

Engine::Engine(const EngineDependencies& deps) noexcept : localization_(this, resolveLocale(deps.args)), scenes_(this) {
  assert(deps.appConfig && "Engine::Engine(): AppConfig cannot be null!");
  registerService<AppConfig>(deps.appConfig);
  assert(deps.args && "Engine::Engine(): ArgsProcessor cannot be null!");
  registerService<const ArgsProcessor>(deps.args);
  assert(deps.gl && "Engine::Engine(): OpenGLApi cannot be null!");
  registerService<const OpenGLApi>(deps.gl);
  assert(deps.platform && "Engine::Engine(): Platform cannot be null!");
  registerService<Platform>(deps.platform);
  assert(deps.viewport && "Engine::Engine(): ViewportManager cannot be null!");
  registerService<ViewportManager>(deps.viewport);

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

  dispatchers_.addListener<EngineEvent::ViewportSizeChange>(
      [viewport = deps.viewport](const auto& event) { viewport->aspect(event.aspect()); });

  platform->addListener<PlatformEvent::OnMonitorChange>(
      [this](const auto& event) { state_.frameInterval(frameInterval(event.monitor)); });
}

void Engine::run() noexcept {
  const auto* platform = service<Platform>();
  const auto* gl = service<const OpenGLApi>();
  time_t startTime = platform->runtime();
  state_.frameInterval(frameInterval(platform->primaryMonitor().value_or(nullptr)));
  while (!state_.isClosing()) {
    scenes_.flush();
    if (auto status = platform->pollEvents(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    do {
      state_.updateRuntime(platform->runtime() - startTime);
      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();
      state_.addTick();
      // dispatchers_.dispatch<EngineEvent::TickComplete>(TickComplete(&state_, runtime())); // TODO: delete me
    } while (state_.runtime() < state_.nextFrame());
    state_.updateRuntime(platform->runtime() - startTime);
    gl->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();
    if (auto status = platform->swapBuffers(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    state_.addFrame();
    state_.scheduleNextFrame();
    // dispatchers_.dispatch<EngineEvent::RenderComplete>(RenderComplete(&state_, runtime())); // TODO: delete me
  }
}

}  // namespace uinta
