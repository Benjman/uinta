#include "uinta/lua/lua_runtime.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "uinta/lua/plugin_loader.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <sol/sol.hpp>  // IWYU pragma: keep
#pragma GCC diagnostic pop

#include "uinta/app_config.h"
#include "uinta/engine/engine.h"
#include "uinta/lua/core_bindings.h"
#include "uinta/lua/ui_bindings.h"

namespace uinta {

namespace fs = std::filesystem;

// ============================================================================
// LuaEventSystem Implementation
// ============================================================================

struct LuaEventSystem::Impl {
  std::unordered_map<std::string, std::vector<sol::function>> listeners;

  void on(const std::string& eventName, sol::function callback) {
    listeners[eventName].push_back(std::move(callback));
  }

  void off(const std::string& eventName, const sol::function& callback) {
    auto it = listeners.find(eventName);
    if (it != listeners.end()) {
      auto& callbacks = it->second;
      std::erase_if(callbacks, [&callback](const sol::function& fn) {
        return fn == callback;
      });
    }
  }

  void emit(const std::string& eventName, const sol::object& data) {
    auto it = listeners.find(eventName);
    if (it != listeners.end()) {
      for (auto& callback : it->second) {
        auto result = callback(data);
        if (!result.valid()) {
          sol::error err = result;
          LOG(ERROR) << "[Lua Event] Error in event '" << eventName
                     << "': " << err.what();
        }
      }
    }
  }

  void clear() { listeners.clear(); }
};

LuaEventSystem::LuaEventSystem() : impl_(std::make_unique<Impl>()) {}
LuaEventSystem::~LuaEventSystem() = default;
LuaEventSystem::LuaEventSystem(LuaEventSystem&&) noexcept = default;
LuaEventSystem& LuaEventSystem::operator=(LuaEventSystem&&) noexcept = default;

void LuaEventSystem::clear() { impl_->clear(); }

LuaEventSystem::Impl& LuaEventSystem::impl() noexcept { return *impl_; }

// ============================================================================
// LuaRuntime Implementation
// ============================================================================

struct LuaRuntime::Impl {
  std::unique_ptr<sol::state> lua;
  Engine* engine = nullptr;
  PluginLoader loader;
  PluginRegistry registry;
  HotReloadManager hotReload;
  LuaEventSystem events;
  std::vector<std::unique_ptr<BindingModule>> bindingModules;
  std::string currentPluginId;
  bool initialized = false;

  // Stage callbacks - one vector per EngineStage
  static constexpr size_t StageCount = 6;
  std::array<std::vector<sol::function>, StageCount> stageCallbacks;

  static size_t stageIndex(EngineStage stage) {
    return static_cast<size_t>(stage);
  }

  void addStageCallback(EngineStage stage, sol::function callback) {
    stageCallbacks[stageIndex(stage)].push_back(std::move(callback));
  }

  void removeStageCallback(EngineStage stage, const sol::function& callback) {
    auto& callbacks = stageCallbacks[stageIndex(stage)];
    std::erase_if(callbacks, [&callback](const sol::function& fn) {
      return fn == callback;
    });
  }

  void clearAllStageCallbacks() {
    for (auto& callbacks : stageCallbacks) {
      callbacks.clear();
    }
  }
};

LuaRuntime::LuaRuntime(Engine* engine, Flags flags)
    : flags_(std::move(flags)), impl_(std::make_unique<Impl>()) {
  impl_->engine = engine;
  impl_->lua = std::make_unique<sol::state>();
}

LuaRuntime::~LuaRuntime() = default;
LuaRuntime::LuaRuntime(LuaRuntime&&) noexcept = default;
LuaRuntime& LuaRuntime::operator=(LuaRuntime&&) noexcept = default;

Status LuaRuntime::initialize() {
  if (impl_->initialized) {
    return OkStatus();
  }

  // Open standard libraries (will be sandboxed)
  impl_->lua->open_libraries(sol::lib::base, sol::lib::package,
                             sol::lib::coroutine, sol::lib::string,
                             sol::lib::table, sol::lib::math, sol::lib::utf8);

  applySandboxing();
  registerCoreBindings();
  registerEventBindings();
  registerConfigBindings();
  registerStageBindings();

  // Register all added binding modules
  for (const auto& module : impl_->bindingModules) {
    module->registerBindings(*impl_->lua);
  }

  impl_->initialized = true;
  LOG(INFO) << "Lua runtime initialized (API version "
            << CURRENT_LUA_API_VERSION.toString() << ")";

  return OkStatus();
}

void LuaRuntime::addBindingModule(std::unique_ptr<BindingModule> module) {
  if (impl_->initialized) {
    // If already initialized, register bindings immediately
    module->registerBindings(*impl_->lua);
  }
  impl_->bindingModules.push_back(std::move(module));
}

Status LuaRuntime::loadPlugins(const std::vector<fs::path>& searchPaths) {
  if (!impl_->initialized) {
    auto status = initialize();
    if (!status.ok()) {
      return status;
    }
  }

  // Discover plugins from search paths
  auto discoverResult = impl_->loader.discoverPlugins(searchPaths);
  if (!discoverResult.ok()) {
    return discoverResult.status();
  }

  std::vector<PluginInfo>& plugins = *discoverResult;

  // Resolve load order
  auto orderResult = impl_->loader.resolveLoadOrder(plugins);
  if (!orderResult.ok()) {
    return orderResult.status();
  }

  // Create a map for quick lookup
  std::unordered_map<std::string, const PluginInfo*> pluginMap;
  for (const auto& plugin : plugins) {
    pluginMap[plugin.id] = &plugin;
  }

  // Load plugins in resolved order
  for (const auto& pluginId : *orderResult) {
    auto it = pluginMap.find(pluginId);
    if (it == pluginMap.end()) {
      continue;
    }

    const PluginInfo& plugin = *it->second;

    // Check plugin API version compatibility
    if (!CURRENT_LUA_API_VERSION.isCompatible(plugin.apiVersion)) {
      handleLuaError(
          "Plugin " + plugin.id,
          absl::StrFormat("Requires API version %s, engine provides %s",
                          plugin.apiVersion.toString(),
                          CURRENT_LUA_API_VERSION.toString()));
      continue;
    }

    auto status = loadPluginScript(plugin);
    if (!status.ok()) {
      handleLuaError("Loading plugin " + plugin.id,
                     std::string(status.message()));
    }
  }

  return OkStatus();
}

Status LuaRuntime::loadPlugin(const std::string& pluginPath) {
  if (!impl_->initialized) {
    auto status = initialize();
    if (!status.ok()) {
      return status;
    }
  }

  auto pluginResult = impl_->loader.loadPluginInfo(pluginPath);
  if (!pluginResult.ok()) {
    return pluginResult.status();
  }

  return loadPluginScript(*pluginResult);
}

Status LuaRuntime::reloadPlugin(const std::string& pluginId) {
  const PluginInfo* info = impl_->registry.getPlugin(pluginId);
  if (info == nullptr) {
    return NotFoundError("Plugin not registered: " + pluginId);
  }

  // Notify binding modules before reload
  for (const auto& module : impl_->bindingModules) {
    module->onBeforeReload();
  }

  // Clear event listeners and stage callbacks for this plugin
  impl_->events.clear();
  clearStageCallbacks();

  // Re-execute the plugin script
  auto status = loadPluginScript(*info);

  // Notify binding modules after reload
  for (const auto& module : impl_->bindingModules) {
    module->onAfterReload();
  }

  impl_->hotReload.clearPendingReload(pluginId);

  return status;
}

Status LuaRuntime::executeScript(const std::string& script,
                                 const std::string& chunkName) {
  if (!impl_->initialized) {
    auto status = initialize();
    if (!status.ok()) {
      return status;
    }
  }

  try {
    auto result =
        impl_->lua->safe_script(script, sol::script_pass_on_error, chunkName);
    if (!result.valid()) {
      sol::error err = result;
      return InternalError(
          absl::StrFormat("Lua error in %s: %s", chunkName, err.what()));
    }
    return OkStatus();
  } catch (const sol::error& e) {
    return InternalError(
        absl::StrFormat("Lua exception in %s: %s", chunkName, e.what()));
  }
}

Status LuaRuntime::executeFile(const std::string& filePath) {
  if (!fs::exists(filePath)) {
    return NotFoundError("Lua file not found: " + filePath);
  }

  std::ifstream file(filePath);
  if (!file.is_open()) {
    return InternalError("Failed to open Lua file: " + filePath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return executeScript(buffer.str(), filePath);
}

HotReloadManager& LuaRuntime::hotReload() noexcept { return impl_->hotReload; }

const HotReloadManager& LuaRuntime::hotReload() const noexcept {
  return impl_->hotReload;
}

PluginRegistry& LuaRuntime::plugins() noexcept { return impl_->registry; }

const PluginRegistry& LuaRuntime::plugins() const noexcept {
  return impl_->registry;
}

LuaEventSystem& LuaRuntime::events() noexcept { return impl_->events; }

const LuaEventSystem& LuaRuntime::events() const noexcept {
  return impl_->events;
}

sol::state& LuaRuntime::state() noexcept { return *impl_->lua; }

const sol::state& LuaRuntime::state() const noexcept { return *impl_->lua; }

void LuaRuntime::update() {
  impl_->hotReload.pollChanges();

  auto pending = impl_->hotReload.getPendingReloads();
  for (const auto& pluginId : pending) {
    auto status = reloadPlugin(pluginId);
    if (!status.ok()) {
      LOG(ERROR) << "Failed to reload plugin '" << pluginId
                 << "': " << status.message();
    }
  }
}

void LuaRuntime::applySandboxing() {
  sol::state& lua = *impl_->lua;

  // Note: We only opened safe libraries (base, package, coroutine, string,
  // table, math, utf8) in initialize(). The following checks handle the case
  // where os/io/debug might exist if the library loading changes in the future.

  // Remove dangerous os functions (if os was loaded)
  sol::object osObj = lua["os"];
  if (osObj.is<sol::table>()) {
    sol::table os = osObj.as<sol::table>();
    os["execute"] = sol::nil;
    os["exit"] = sol::nil;
    os["remove"] = sol::nil;
    os["rename"] = sol::nil;
    os["setenv"] = sol::nil;
    os["setlocale"] = sol::nil;
    os["tmpname"] = sol::nil;
  }

  // Remove io library entirely (if loaded)
  lua["io"] = sol::nil;

  // Remove dangerous loading functions
  lua["loadfile"] = sol::nil;
  lua["dofile"] = sol::nil;

  // Remove debug library entirely (if loaded)
  lua["debug"] = sol::nil;

  // Remove package.loadlib (allows loading C libraries)
  sol::object pkgObj = lua["package"];
  if (pkgObj.is<sol::table>()) {
    sol::table package = pkgObj.as<sol::table>();
    package["loadlib"] = sol::nil;
  }

  LOG(INFO) << "Lua sandboxing applied";
}

void LuaRuntime::registerCoreBindings() {
  // Register built-in core bindings
  auto coreBindings = std::make_unique<CoreBindings>(impl_->engine);
  coreBindings->registerBindings(*impl_->lua);

  // Register stub UI bindings
  auto uiBindings = std::make_unique<UIBindings>();
  uiBindings->registerBindings(*impl_->lua);
}

void LuaRuntime::registerEventBindings() {
  sol::state& lua = *impl_->lua;
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table events = uinta["events"].get_or_create<sol::table>();

  auto& eventImpl = impl_->events.impl();

  events["on"] = [&eventImpl](const std::string& name, sol::function callback) {
    eventImpl.on(name, std::move(callback));
  };

  events["off"] = [&eventImpl](const std::string& name,
                               const sol::function& callback) {
    eventImpl.off(name, callback);
  };

  events["emit"] = [&eventImpl](const std::string& name,
                                const sol::object& data) {
    eventImpl.emit(name, data);
  };
}

void LuaRuntime::registerConfigBindings() {
  if (impl_->engine == nullptr) {
    LOG(WARNING) << "Cannot register config bindings: no engine pointer";
    return;
  }

  auto* config = impl_->engine->service<AppConfig>();
  if (config == nullptr) {
    LOG(WARNING) << "Cannot register config bindings: engine has no AppConfig";
    return;
  }

  sol::state& lua = *impl_->lua;
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table configTable = uinta["config"].get_or_create<sol::table>();

  // Read functions (only if configReadEnabled)
  if (flags_.configReadEnabled()) {
    configTable["getString"] =
        [config](const std::string& key) -> sol::optional<std::string> {
      auto result = config->getString(key);
      if (result) {
        return *result;
      }
      return sol::nullopt;
    };

    configTable["getInt"] =
        [config](const std::string& key) -> sol::optional<i32> {
      auto result = config->getInt(key);
      if (result) {
        return *result;
      }
      return sol::nullopt;
    };

    configTable["getFloat"] =
        [config](const std::string& key) -> sol::optional<f32> {
      auto result = config->getFloat(key);
      if (result) {
        return *result;
      }
      return sol::nullopt;
    };

    configTable["getBoolean"] =
        [config](const std::string& key) -> sol::optional<bool> {
      auto result = config->getBoolean(key);
      if (result) {
        return *result;
      }
      return sol::nullopt;
    };

    LOG(INFO) << "Lua config read bindings registered";
  }

  // Write functions (only if configWriteEnabled)
  if (flags_.configWriteEnabled()) {
    configTable["setString"] = [config](const std::string& key,
                                        const std::string& value) -> bool {
      return config->setString(key, value).ok();
    };

    configTable["setInt"] = [config](const std::string& key,
                                     i32 value) -> bool {
      return config->setInt(key, value).ok();
    };

    configTable["setFloat"] = [config](const std::string& key,
                                       f32 value) -> bool {
      return config->setFloat(key, value).ok();
    };

    configTable["setBoolean"] = [config](const std::string& key,
                                         bool value) -> bool {
      return config->setBoolean(key, value).ok();
    };

    configTable["flush"] = [config]() { config->flush(); };

    LOG(INFO) << "Lua config write bindings registered";
  }
}

void LuaRuntime::registerPluginInfoBindings(const PluginInfo& plugin) {
  sol::state& lua = *impl_->lua;
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table pluginTable = uinta["plugin"].get_or_create<sol::table>();

  pluginTable["id"] = plugin.id;
  pluginTable["version"] = plugin.version;
  pluginTable["path"] = plugin.path;
  pluginTable["name"] = plugin.name;
  pluginTable["author"] = plugin.author;
}

Status LuaRuntime::loadPluginScript(const PluginInfo& plugin) {
  fs::path initLua = fs::path(plugin.path) / "init.lua";

  if (!fs::exists(initLua)) {
    return NotFoundError("Plugin init.lua not found: " + initLua.string());
  }

  // Set current plugin context
  impl_->currentPluginId = plugin.id;
  registerPluginInfoBindings(plugin);

  // Configure package.path for this plugin's require() calls
  configurePackagePath(plugin);

  // Execute the plugin script
  auto status = executeFile(initLua.string());
  if (!status.ok()) {
    impl_->currentPluginId.clear();
    return status;
  }

  // Register the plugin
  impl_->registry.registerPlugin(plugin);

  // Register plugin files for hot reload
  impl_->hotReload.registerPluginFile(initLua.string(), plugin.id);

  // Notify binding modules
  for (const auto& module : impl_->bindingModules) {
    module->onPluginLoaded(plugin);
  }

  impl_->currentPluginId.clear();
  LOG(INFO) << "Loaded plugin: " << plugin.name << " (" << plugin.id << " v"
            << plugin.version << ")";

  return OkStatus();
}

void LuaRuntime::configurePackagePath(const PluginInfo& plugin) {
  sol::state& lua = *impl_->lua;

  fs::path pluginDir = fs::path(plugin.path);
  fs::path libDir = pluginDir / "lib";

  // Build package.path with:
  // 1. Plugin's lib/ directory
  // 2. Plugin's root directory
  // 3. Preserve existing path for Lua standard modules
  std::string newPath;

  // Add plugin lib directory (for require("utils") -> lib/utils.lua)
  newPath += libDir.string() + "/?.lua;";

  // Add plugin root directory (for require("init") -> init.lua)
  newPath += pluginDir.string() + "/?.lua;";

  // Append existing package.path
  sol::object existingPath = lua["package"]["path"];
  if (existingPath.is<std::string>()) {
    newPath += existingPath.as<std::string>();
  }

  lua["package"]["path"] = newPath;
}

// NOLINTNEXTLINE
void LuaRuntime::handleLuaError(const std::string& context,
                                const std::string& error) {
#ifdef UINTA_DEBUG
  LOG(FATAL) << "[Lua] " << context << ": " << error;
#else
  LOG(ERROR) << "[Lua] " << context << ": " << error;
  // In release mode, disable the plugin if it's being loaded
  if (!impl_->currentPluginId.empty()) {
    impl_->registry.disablePlugin(impl_->currentPluginId);
    LOG(WARNING) << "Plugin '" << impl_->currentPluginId
                 << "' has been disabled due to error";
  }
#endif
}

void LuaRuntime::registerStageBindings() {
  sol::state& lua = *impl_->lua;
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();

  // Helper to create on/off pair for a stage
  auto registerStage = [this, &uinta](const char* onName, const char* offName,
                                      EngineStage stage) {
    uinta[onName] = [this, stage](sol::function callback) {
      impl_->addStageCallback(stage, std::move(callback));
    };
    uinta[offName] = [this, stage](const sol::function& callback) {
      impl_->removeStageCallback(stage, callback);
    };
  };

  registerStage("onPreTick", "offPreTick", EngineStage::PreTick);
  registerStage("onTick", "offTick", EngineStage::Tick);
  registerStage("onPostTick", "offPostTick", EngineStage::PostTick);
  registerStage("onPreRender", "offPreRender", EngineStage::PreRender);
  registerStage("onRender", "offRender", EngineStage::Render);
  registerStage("onPostRender", "offPostRender", EngineStage::PostRender);

  LOG(INFO) << "Lua stage bindings registered";
}

void LuaRuntime::dispatchStage(EngineStage stage, f32 deltaTime) {
  if (!impl_ || !impl_->initialized) {
    return;
  }

  auto& callbacks =
      impl_->stageCallbacks[uinta::LuaRuntime::Impl::stageIndex(stage)];
  for (auto& callback : callbacks) {
    auto result = callback(deltaTime);
    if (!result.valid()) {
      sol::error err = result;
      LOG(ERROR) << "[Lua] Stage callback error: " << err.what();
    }
  }
}

void LuaRuntime::clearStageCallbacks() { impl_->clearAllStageCallbacks(); }

}  // namespace uinta
