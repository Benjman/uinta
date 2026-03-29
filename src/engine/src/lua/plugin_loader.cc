#include "uinta/lua/plugin_loader.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <queue>
#include <unordered_set>

namespace uinta {

namespace fs = std::filesystem;

StatusOr<std::vector<PluginInfo>> PluginLoader::discoverPlugins(
    const std::vector<fs::path>& searchPaths) const {
  std::vector<PluginInfo> result;
  std::unordered_set<std::string> seenIds;

  for (const auto& searchPath : searchPaths) {
    if (!fs::exists(searchPath)) {
      LOG(WARNING) << "Plugin search path does not exist: "
                   << searchPath.string();
      continue;
    }

    if (!fs::is_directory(searchPath)) {
      LOG(WARNING) << "Plugin search path is not a directory: "
                   << searchPath.string();
      continue;
    }

    for (const auto& entry : fs::directory_iterator(searchPath)) {
      if (!entry.is_directory()) {
        continue;
      }

      fs::path subdir = entry.path();

      // Skip directories with a .disabled marker
      if (fs::exists(subdir / ".disabled")) {
        LOG(INFO) << "Skipping disabled plugin: " << subdir.filename().string();
        continue;
      }

      // Skip directories without plugin.yaml
      if (!fs::exists(subdir / "plugin.yaml")) {
        continue;
      }

      auto pluginResult = loadPluginInfo(subdir.string());
      if (!pluginResult.ok()) {
        LOG(WARNING) << "Failed to load plugin info from " << subdir.string()
                     << ": " << pluginResult.status().message();
        continue;
      }

      const auto& id = pluginResult->id;

      // First-found wins for duplicate IDs
      if (seenIds.contains(id)) {
        LOG(INFO) << "Skipping duplicate plugin '" << id << "' found in "
                  << searchPath.string();
        continue;
      }

      seenIds.insert(id);
      result.push_back(std::move(*pluginResult));
    }
  }

  return result;
}

StatusOr<PluginInfo> PluginLoader::loadPluginInfo(
    const std::string& pluginPath) const {
  fs::path pluginDir(pluginPath);
  fs::path pluginYaml = pluginDir / "plugin.yaml";

  if (!fs::exists(pluginYaml)) {
    return NotFoundError(
        absl::StrFormat("Plugin metadata not found: %s", pluginYaml.string()));
  }

  try {
    YAML::Node root = YAML::LoadFile(pluginYaml.string());

    PluginInfo info;
    info.path = fs::canonical(pluginDir).string();

    if (root["id"]) {
      info.id = root["id"].as<std::string>();
    } else {
      return InvalidArgumentError(
          absl::StrFormat("Plugin missing 'id' field: %s", pluginPath));
    }

    if (root["name"]) {
      info.name = root["name"].as<std::string>();
    } else {
      info.name = info.id;
    }

    if (root["version"]) {
      info.version = root["version"].as<std::string>();
    }

    if (root["api_version"]) {
      info.apiVersion =
          ApiVersion::parse(root["api_version"].as<std::string>());
    }

    if (root["author"]) {
      info.author = root["author"].as<std::string>();
    }

    if (root["dependencies"] && root["dependencies"].IsSequence()) {
      for (const auto& dep : root["dependencies"]) {
        info.dependencies.push_back(dep.as<std::string>());
      }
    }

    if (root["load_order"]) {
      info.loadOrder = root["load_order"].as<i32>();
    }

    return info;
  } catch (const YAML::Exception& e) {
    return InternalError(
        absl::StrFormat("Failed to parse plugin.yaml: %s", e.what()));
  }
}

StatusOr<std::vector<std::string>> PluginLoader::resolveLoadOrder(
    const std::vector<PluginInfo>& plugins) const {
  // Build adjacency list and in-degree map for topological sort
  std::unordered_map<std::string, std::vector<std::string>> dependents;
  std::unordered_map<std::string, int> inDegree;
  std::unordered_map<std::string, i32> loadOrders;

  // Initialize
  for (const auto& plugin : plugins) {
    inDegree[plugin.id] = 0;
    loadOrders[plugin.id] = plugin.loadOrder;
  }

  // Build dependency graph
  for (const auto& plugin : plugins) {
    for (const auto& dep : plugin.dependencies) {
      if (!inDegree.contains(dep)) {
        return InvalidArgumentError(absl::StrFormat(
            "Plugin '%s' depends on unknown plugin '%s'", plugin.id, dep));
      }
      dependents[dep].push_back(plugin.id);
      inDegree[plugin.id]++;
    }
  }

  // Kahn's algorithm with priority queue (respecting load_order)
  auto compare = [&loadOrders](const std::string& a, const std::string& b) {
    return loadOrders[a] > loadOrders[b];  // Min-heap
  };
  std::priority_queue<std::string, std::vector<std::string>, decltype(compare)>
      ready(compare);

  for (const auto& [id, degree] : inDegree) {
    if (degree == 0) {
      ready.push(id);
    }
  }

  std::vector<std::string> result;
  result.reserve(plugins.size());

  while (!ready.empty()) {
    std::string current = ready.top();
    ready.pop();
    result.push_back(current);

    for (const auto& dependent : dependents[current]) {
      inDegree[dependent]--;
      if (inDegree[dependent] == 0) {
        ready.push(dependent);
      }
    }
  }

  if (result.size() != plugins.size()) {
    return InvalidArgumentError("Circular dependency detected in plugins");
  }

  return result;
}

}  // namespace uinta
