#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_HOT_RELOAD_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_HOT_RELOAD_H_

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "uinta/status.h"

namespace uinta {

/// Callback type for reload events.
using ReloadCallback = std::function<void(const std::string& pluginId)>;

/// Manages file watching and hot reload for Lua plugins.
/// Uses platform-specific APIs: inotify (Linux), FSEvents (macOS),
/// ReadDirectoryChangesW (Windows).
class HotReloadManager {
 public:
  HotReloadManager();
  ~HotReloadManager();

  HotReloadManager(const HotReloadManager&) = delete;
  HotReloadManager& operator=(const HotReloadManager&) = delete;
  HotReloadManager(HotReloadManager&&) = delete;
  HotReloadManager& operator=(HotReloadManager&&) = delete;

  /// Start watching a directory for changes.
  /// @param directory Path to watch (typically the plugins directory).
  /// @return Status indicating success or failure.
  [[nodiscard]] Status watchDirectory(const std::string& directory);

  /// Stop watching a directory.
  /// @param directory Path to stop watching.
  void unwatchDirectory(const std::string& directory);

  /// Stop watching all directories.
  void unwatchAll();

  /// Register a callback to be called when a plugin file changes.
  /// @param callback Function to call with the changed plugin ID.
  void onReload(ReloadCallback callback);

  /// Check for pending file changes and process them.
  /// Call this from the main thread each frame.
  void pollChanges();

  /// Get the list of plugin IDs that have pending changes.
  /// @return Vector of plugin IDs with changes.
  [[nodiscard]] std::vector<std::string> getPendingReloads();

  /// Clear pending reloads for a specific plugin.
  /// @param pluginId Plugin to clear.
  void clearPendingReload(const std::string& pluginId);

  /// Check if hot reload is currently active (watching any directories).
  [[nodiscard]] bool isActive() const noexcept;

  /// Associate a file path with a plugin ID for change detection.
  /// @param filePath Path to the file (e.g., "plugins/my_plugin/init.lua").
  /// @param pluginId The plugin that owns this file.
  void registerPluginFile(const std::string& filePath,
                          const std::string& pluginId);

  /// Remove file associations for a plugin.
  /// @param pluginId Plugin to unregister.
  void unregisterPluginFiles(const std::string& pluginId);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;

  std::vector<ReloadCallback> callbacks_;
  std::unordered_map<std::string, std::string>
      fileToPlugin_;  // file -> pluginId
  std::unordered_set<std::string> pendingReloads_;
  mutable std::mutex mutex_;
  std::atomic<bool> active_{false};

  void notifyChange(const std::string& filePath);
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_HOT_RELOAD_H_
