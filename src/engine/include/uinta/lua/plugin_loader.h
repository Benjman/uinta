#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_LOADER_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_LOADER_H_

#include <filesystem>
#include <string>
#include <vector>

#include "uinta/lua/plugin_info.h"
#include "uinta/status.h"

namespace uinta {

/// Discovers plugins from search paths and loads plugin metadata from YAML
/// files.
class PluginLoader {
 public:
  PluginLoader() = default;
  ~PluginLoader() = default;

  PluginLoader(const PluginLoader&) = delete;
  PluginLoader& operator=(const PluginLoader&) = delete;
  PluginLoader(PluginLoader&&) = default;
  PluginLoader& operator=(PluginLoader&&) = default;

  /// Discover plugins by scanning search paths for subdirectories containing
  /// plugin.yaml. Paths are scanned in order; first-found-wins for duplicate
  /// plugin IDs. Subdirectories containing a `.disabled` file are skipped.
  /// @param searchPaths Ordered list of directories to scan.
  /// @return Discovered plugin info list or error status.
  [[nodiscard]] StatusOr<std::vector<PluginInfo>> discoverPlugins(
      const std::vector<std::filesystem::path>& searchPaths) const;

  /// Load plugin metadata from a plugin directory.
  /// @param pluginPath Path to plugin directory containing plugin.yaml.
  /// @return Parsed plugin info or error status.
  [[nodiscard]] StatusOr<PluginInfo> loadPluginInfo(
      const std::string& pluginPath) const;

  /// Resolve plugin load order based on dependencies.
  /// Performs topological sort to ensure dependencies are loaded first.
  /// @param plugins List of plugins to sort.
  /// @return Sorted list of plugin IDs, or error if circular dependency found.
  [[nodiscard]] StatusOr<std::vector<std::string>> resolveLoadOrder(
      const std::vector<PluginInfo>& plugins) const;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_LOADER_H_
