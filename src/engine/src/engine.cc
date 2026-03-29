#include "uinta/engine/engine.h"

#include <absl/log/log.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <absl/strings/str_split.h>

#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

#include "uinta/app_config.h"
#include "uinta/args.h"
#include "uinta/cfg.h"
#include "uinta/gl.h"
#include "uinta/input/input_frame_gurad.h"
#include "uinta/input/input_system.h"
#include "uinta/localization/locale.h"
#include "uinta/localization/localization_system.h"
#include "uinta/lua/lua_runtime.h"
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

Engine::Engine(const EngineDependencies& deps) noexcept
    : platform_(deps.platform), localization_(resolveLocale(deps.args)), scenes_(this) {
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

  registerService<LocalizationSystem>(&localization_);
  registerService<SceneSystem>(&scenes_);
  registerService<InputSystem>(&inputSystem_);

  platform_->engine(this);
  platform_->addListener<PlatformEvent::OnCloseRequest>([this](const auto&) { state_.isClosing(true); });
  platform_->addListener<PlatformEvent::OnError>(
      [](const auto& event) { LOG(FATAL) << absl::StrFormat("%i: %s", event.code, event.description); });

  platform_->addListener<PlatformEvent::OnDebugMessage>([](const auto& event) {
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

  platform_->addListener<PlatformEvent::OnViewportSizeChange>([this](const auto&) {
    auto width = platform_->window()->width();
    auto height = platform_->window()->height();
    const auto* gl = service<const OpenGLApi>();
    gl->viewport(0, 0, static_cast<i32>(width), static_cast<i32>(height));
    dispatchers_.dispatch<EngineEvent::ViewportSizeChange>(ViewportSizeChange(width, height));
    LOG(INFO) << absl::StrFormat("Event: Viewport size change (%u, %u)", width, height);
  });

  dispatchers_.addListener<EngineEvent::ViewportSizeChange>(
      [viewport = deps.viewport](const auto& event) { viewport->aspect(event.aspect()); });

  platform_->addListener<PlatformEvent::OnMonitorChange>(
      [this](const auto& event) { state_.frameInterval(frameInterval(event.monitor)); });

  if (auto status = platform_->registerInputHandlers(inputSystem_.input()); !status.ok()) {
    LOG(FATAL) << status.message();
  }

  // Initialize Lua runtime
  lua_ = LuaRuntime(this);
  registerService<LuaRuntime>(&lua_);
  if (auto status = lua_.initialize(); !status.ok()) {
    LOG(ERROR) << "Failed to initialize Lua runtime: " << status.message();
  } else {
    std::vector<std::filesystem::path> pluginPaths;

    const auto* args = service<const ArgsProcessor>();
    assert(args && "Engine::Engine(): ArgsProcessor cannot be null!");

    // CLI --plugin-path (highest priority)
    if (auto val = args->getValue(ArgsProcessor::PluginPath)) {
      for (auto piece : absl::StrSplit(*val, ';', absl::SkipEmpty())) {
        pluginPaths.emplace_back(std::string(piece));
      }
    }

#ifdef UINTA_DEBUG
    // Source-tree plugins (debug only)
    pluginPaths.emplace_back(cfg::UINTA_SRC_PLUGINS_DIR);
#endif

    // System/installed plugins
    pluginPaths.emplace_back(cfg::UINTA_INSTALL_PLUGINS_DIR);

    if (auto status = lua_.loadPlugins(pluginPaths); !status.ok()) {
      LOG(ERROR) << "Failed to load plugins: " << status.message();
    }
  }
}

void Engine::run() noexcept {
  const auto* gl = service<const OpenGLApi>();
  state_.frameInterval(frameInterval(platform_->primaryMonitor().value_or(nullptr)));
  while (!state_.isClosing()) {
    scenes_.flush();
    InputFrameGuard inputGuard(this, state().delta());
    do {
      state_.updateRuntime(runtime());
      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();
      state_.addTick();
      dispatchers_.dispatch<EngineEvent::TickComplete>(TickComplete(&state_, runtime()));
    } while (state_.runtime() < state_.nextFrame());
    state_.updateRuntime(runtime());
    lua_.update();
    gl->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();
    if (auto status = platform_->swapBuffers(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    state_.addFrame();
    state_.scheduleNextFrame();
    dispatchers_.dispatch<EngineEvent::RenderComplete>(RenderComplete(&state_, runtime()));
  }
}

}  // namespace uinta
