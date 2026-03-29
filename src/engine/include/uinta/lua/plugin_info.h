#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_INFO_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_INFO_H_

#include <string>
#include <vector>

#include "uinta/lua/api_version.h"
#include "uinta/types.h"

namespace uinta {

/// Metadata for a Lua plugin, loaded from plugin.yaml.
struct PluginInfo {
  std::string id;                         ///< Unique plugin identifier
  std::string name;                       ///< Human-readable name
  std::string version;                    ///< Plugin version string
  ApiVersion apiVersion = {0, 0, 0};      ///< Required API version
  std::string author;                     ///< Plugin author
  std::string path;                       ///< Path to plugin directory
  std::vector<std::string> dependencies;  ///< IDs of required plugins
  i32 loadOrder = 0;                      ///< Load priority (lower = earlier)
  bool enabled = true;                    ///< Whether the plugin is active
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_PLUGIN_INFO_H_
