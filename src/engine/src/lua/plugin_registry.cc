#include "uinta/lua/plugin_registry.h"

#include <algorithm>

namespace uinta {

void PluginRegistry::registerPlugin(PluginInfo info) noexcept {
  plugins_[info.id] = std::move(info);
}

void PluginRegistry::unregisterPlugin(const std::string& id) noexcept {
  plugins_.erase(id);
}

bool PluginRegistry::hasPlugin(const std::string& id) const noexcept {
  return plugins_.contains(id);
}

const PluginInfo* PluginRegistry::getPlugin(
    const std::string& id) const noexcept {
  auto it = plugins_.find(id);
  if (it == plugins_.end()) {
    return nullptr;
  }
  return &it->second;
}

std::vector<const PluginInfo*> PluginRegistry::getAllPlugins() const noexcept {
  std::vector<const PluginInfo*> result;
  result.reserve(plugins_.size());
  for (const auto& [id, info] : plugins_) {
    result.push_back(&info);
  }
  return result;
}

std::vector<const PluginInfo*> PluginRegistry::getPluginsInLoadOrder()
    const noexcept {
  auto plugins = getAllPlugins();
  std::ranges::sort(plugins.begin(), plugins.end(),
                    [](const PluginInfo* a, const PluginInfo* b) {
                      return a->loadOrder < b->loadOrder;
                    });
  return plugins;
}

void PluginRegistry::clear() noexcept { plugins_.clear(); }

void PluginRegistry::disablePlugin(const std::string& id) noexcept {
  auto it = plugins_.find(id);
  if (it != plugins_.end()) {
    it->second.enabled = false;
  }
}

}  // namespace uinta
