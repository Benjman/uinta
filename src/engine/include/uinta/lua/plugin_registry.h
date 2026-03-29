#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_REGISTRY_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_REGISTRY_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "uinta/lua/plugin_info.h"

namespace uinta {

/// Registry for tracking loaded Lua plugins.
/// Maintains plugin metadata and load state.
class PluginRegistry {
 public:
  PluginRegistry() = default;
  ~PluginRegistry() = default;

  PluginRegistry(const PluginRegistry&) = delete;
  PluginRegistry& operator=(const PluginRegistry&) = delete;
  PluginRegistry(PluginRegistry&&) = default;
  PluginRegistry& operator=(PluginRegistry&&) = default;

  /// Register a plugin as loaded.
  /// @param info Plugin metadata.
  void registerPlugin(PluginInfo info) noexcept;

  /// Unregister a plugin.
  /// @param id Plugin identifier.
  void unregisterPlugin(const std::string& id) noexcept;

  /// Check if a plugin is registered.
  /// @param id Plugin identifier.
  /// @return true if the plugin is registered.
  [[nodiscard]] bool hasPlugin(const std::string& id) const noexcept;

  /// Get information about a registered plugin.
  /// @param id Plugin identifier.
  /// @return Pointer to plugin info, or nullptr if not found.
  [[nodiscard]] const PluginInfo* getPlugin(
      const std::string& id) const noexcept;

  /// Get all registered plugins.
  /// @return Vector of all plugin info.
  [[nodiscard]] std::vector<const PluginInfo*> getAllPlugins() const noexcept;

  /// Get plugins in load order.
  /// @return Vector of plugin info sorted by load order.
  [[nodiscard]] std::vector<const PluginInfo*> getPluginsInLoadOrder()
      const noexcept;

  /// Get the number of registered plugins.
  [[nodiscard]] size_t size() const noexcept { return plugins_.size(); }

  /// Check if the registry is empty.
  [[nodiscard]] bool empty() const noexcept { return plugins_.empty(); }

  /// Clear all registered plugins.
  void clear() noexcept;

  /// Mark a plugin as disabled (e.g., after a load error in release mode).
  /// @param id Plugin identifier.
  void disablePlugin(const std::string& id) noexcept;

 private:
  std::unordered_map<std::string, PluginInfo> plugins_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_REGISTRY_H_
