#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_LUA_RUNTIME_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_LUA_RUNTIME_H_

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "uinta/engine/engine_stage.h"
#include "uinta/flags.h"
#include "uinta/lua/api_version.h"
#include "uinta/lua/binding_module.h"
#include "uinta/lua/hot_reload.h"
#include "uinta/lua/plugin_info.h"
#include "uinta/lua/plugin_registry.h"
#include "uinta/status.h"
#include "uinta/types.h"

namespace sol {
class state;
}  // namespace sol

namespace uinta {

class Engine;

/// Event system for Lua scripts.
/// Allows scripts to subscribe to and emit events.
/// Implementation is in lua_runtime.cc using sol2 types.
class LuaEventSystem {
 public:
  LuaEventSystem();
  ~LuaEventSystem();

  LuaEventSystem(const LuaEventSystem&) = delete;
  LuaEventSystem& operator=(const LuaEventSystem&) = delete;
  LuaEventSystem(LuaEventSystem&&) noexcept;
  LuaEventSystem& operator=(LuaEventSystem&&) noexcept;

  /// Clear all event subscriptions.
  void clear();

  /// Get access to the internal implementation for Lua binding.
  /// Used internally by LuaRuntime.
  struct Impl;
  [[nodiscard]] Impl& impl() noexcept;

 private:
  std::unique_ptr<Impl> impl_;
};

/// Main Lua scripting runtime for the Uinta engine.
/// Manages the Lua state, plugin loading, hot reload, and bindings.
///
/// Thread Safety: LuaRuntime is NOT thread-safe and must only be called
/// from the main/game thread.
///
/// Example usage:
/// @code
/// uinta::LuaRuntime lua;
/// lua.addBindingModule(std::make_unique<MyGameBindings>(gameState));
/// lua.loadPlugins({"/path/to/plugins"});
///
/// // Enable hot reload for development
/// lua.hotReload().watchDirectory("plugins/");
/// lua.hotReload().onReload([](const std::string& plugin) {
///   LOG(INFO) << "Reloaded: " << plugin;
/// });
/// @endcode
class LuaRuntime {
 public:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(0);

    /// Whether plugins can read from AppConfig.
    [[nodiscard]] bool configReadEnabled() const noexcept {
      return (flags_ & ConfigReadMask) != 0;
    }
    void configReadEnabled(bool v) noexcept {
      flags_ = (flags_ & ~ConfigReadMask) | (v ? ConfigReadMask : 0);
    }

    /// Whether plugins can write to AppConfig.
    [[nodiscard]] bool configWriteEnabled() const noexcept {
      return (flags_ & ConfigWriteMask) != 0;
    }
    void configWriteEnabled(bool v) noexcept {
      flags_ = (flags_ & ~ConfigWriteMask) | (v ? ConfigWriteMask : 0);
    }

    /// Whether hot reload is enabled.
    [[nodiscard]] bool hotReloadEnabled() const noexcept {
      return (flags_ & HotReloadMask) != 0;
    }
    void hotReloadEnabled(bool v) noexcept {
      flags_ = (flags_ & ~HotReloadMask) | (v ? HotReloadMask : 0);
    }

   private:
    static constexpr value_type ConfigReadMask = 1 << 0;
    static constexpr value_type ConfigWriteMask = 1 << 1;
    static constexpr value_type HotReloadMask = 1 << 2;

    value_type flags_;
  };

  /// Construct a new Lua runtime.
  /// @param engine Optional engine pointer for core bindings.
  /// @param flags Feature flags controlling runtime behavior.
  explicit LuaRuntime(Engine* engine = nullptr, Flags flags = {});
  ~LuaRuntime();

  LuaRuntime(const LuaRuntime&) = delete;
  LuaRuntime& operator=(const LuaRuntime&) = delete;
  LuaRuntime(LuaRuntime&&) noexcept;
  LuaRuntime& operator=(LuaRuntime&&) noexcept;

  /// Initialize the Lua runtime.
  /// Sets up sandboxing and registers core bindings.
  /// @return Status indicating success or failure.
  [[nodiscard]] Status initialize();

  /// Add a binding module to register custom Lua APIs.
  /// Modules are registered in the order they are added.
  /// @param module The binding module to add.
  void addBindingModule(std::unique_ptr<BindingModule> module);

  /// Discover and load plugins from search paths.
  /// Scans each path for subdirectories containing plugin.yaml, resolves
  /// dependencies, and loads plugins in topological order.
  /// @param searchPaths Ordered directories to scan (first = highest priority).
  /// @return Status indicating success or failure.
  [[nodiscard]] Status loadPlugins(
      const std::vector<std::filesystem::path>& searchPaths);

  /// Load a single plugin from a directory.
  /// @param pluginPath Path to plugin directory.
  /// @return Status indicating success or failure.
  [[nodiscard]] Status loadPlugin(const std::string& pluginPath);

  /// Reload a plugin by ID.
  /// @param pluginId The plugin to reload.
  /// @return Status indicating success or failure.
  [[nodiscard]] Status reloadPlugin(const std::string& pluginId);

  /// Execute a Lua script string.
  /// @param script The Lua code to execute.
  /// @param chunkName Optional name for error messages.
  /// @return Status indicating success or failure.
  [[nodiscard]] Status executeScript(const std::string& script,
                                     const std::string& chunkName = "chunk");

  /// Execute a Lua script file.
  /// @param filePath Path to the Lua file.
  /// @return Status indicating success or failure.
  [[nodiscard]] Status executeFile(const std::string& filePath);

  /// Get the hot reload manager.
  /// @return Reference to the hot reload manager.
  [[nodiscard]] HotReloadManager& hotReload() noexcept;
  [[nodiscard]] const HotReloadManager& hotReload() const noexcept;

  /// Get the plugin registry.
  /// @return Reference to the plugin registry.
  [[nodiscard]] PluginRegistry& plugins() noexcept;
  [[nodiscard]] const PluginRegistry& plugins() const noexcept;

  /// Get the event system.
  /// @return Reference to the event system.
  [[nodiscard]] LuaEventSystem& events() noexcept;
  [[nodiscard]] const LuaEventSystem& events() const noexcept;

  /// Get the underlying sol2 state.
  /// Use with caution - prefer using binding modules.
  /// @return Reference to the sol2 state.
  [[nodiscard]] sol::state& state() noexcept;
  [[nodiscard]] const sol::state& state() const noexcept;

  /// Check for and process hot reload changes.
  /// Call this each frame from the main thread.
  void update();

  /// Dispatch callbacks for an engine stage.
  /// @param stage The engine stage being executed.
  /// @param deltaTime Time since last frame (seconds).
  void dispatchStage(EngineStage stage, f32 deltaTime);

  /// Get the current API version.
  [[nodiscard]] static constexpr ApiVersion apiVersion() noexcept {
    return CURRENT_LUA_API_VERSION;
  }

  /// Get the feature flags.
  [[nodiscard]] Flags& flags() noexcept { return flags_; }
  [[nodiscard]] const Flags& flags() const noexcept { return flags_; }

 private:
  Flags flags_;
  struct Impl;
  std::unique_ptr<Impl> impl_;

  void applySandboxing();
  void registerCoreBindings();
  void registerEventBindings();
  void registerConfigBindings();
  void registerStageBindings();
  void registerPluginInfoBindings(const PluginInfo& plugin);
  void configurePackagePath(const PluginInfo& plugin);
  [[nodiscard]] Status loadPluginScript(const PluginInfo& plugin);
  void handleLuaError(const std::string& context, const std::string& error);
  void clearStageCallbacks();
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_LUA_RUNTIME_H_
