#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_BINDING_MODULE_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_BINDING_MODULE_H_

#include "uinta/lua/plugin_info.h"

// Forward declare sol::state to avoid including sol2 in the header
namespace sol {
class state;
}  // namespace sol

namespace uinta {

/// Interface for registering custom Lua bindings.
/// Apps implement this to expose game-specific APIs to Lua scripts.
///
/// Example usage:
/// @code
/// class MyGameBindings : public BindingModule {
///  public:
///   void registerBindings(sol::state& lua) override {
///     lua["game"]["player"]["health"] = [this]() { return playerHealth_; };
///   }
/// };
/// @endcode
class BindingModule {
 public:
  virtual ~BindingModule() = default;

  BindingModule() = default;
  BindingModule(const BindingModule&) = default;
  BindingModule& operator=(const BindingModule&) = default;
  BindingModule(BindingModule&&) = default;
  BindingModule& operator=(BindingModule&&) = default;

  /// Called once when LuaRuntime initializes to register bindings.
  /// @param lua The sol2 state to register bindings on.
  virtual void registerBindings(sol::state& lua) = 0;

  /// Called after a plugin is successfully loaded.
  /// Override to perform plugin-specific initialization.
  /// @param plugin Information about the loaded plugin.
  virtual void onPluginLoaded(const PluginInfo& plugin) {
    (void)plugin;  // Suppress unused parameter warning
  }

  /// Called before hot reload begins.
  /// Override to save any state that needs to persist across reload.
  virtual void onBeforeReload() {}

  /// Called after hot reload completes successfully.
  /// Override to restore state or re-initialize bindings.
  virtual void onAfterReload() {}
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_BINDING_MODULE_H_
