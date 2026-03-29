#include "uinta/lua/hot_reload.h"

#include <absl/log/log.h>

#include <filesystem>

#if defined(__linux__)
#include <sys/inotify.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#elif defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace uinta {

namespace fs = std::filesystem;

struct HotReloadManager::Impl {
#if defined(__linux__)
  int inotifyFd = -1;
  std::unordered_map<int, std::string> watchDescriptors;  // wd -> directory
  std::unordered_map<std::string, int> directoryToWd;     // directory -> wd
#elif defined(__APPLE__)
  FSEventStreamRef eventStream = nullptr;
  CFRunLoopRef runLoop = nullptr;
  std::unordered_set<std::string> watchedDirectories;
#elif defined(_WIN32)
  std::unordered_map<std::string, HANDLE> directoryHandles;
  std::vector<OVERLAPPED> overlappeds;
  std::vector<std::vector<char> > buffers;
#endif
};

HotReloadManager::HotReloadManager() : impl_(std::make_unique<Impl>()) {
#if defined(__linux__)
  impl_->inotifyFd = inotify_init1(IN_NONBLOCK);
  if (impl_->inotifyFd < 0) {
    LOG(WARNING) << "Failed to initialize inotify: " << strerror(errno);
  }
#endif
}

HotReloadManager::~HotReloadManager() {
  unwatchAll();
#if defined(__linux__)
  if (impl_->inotifyFd >= 0) {
    close(impl_->inotifyFd);
  }
#endif
}

Status HotReloadManager::watchDirectory(const std::string& directory) {
  if (!fs::exists(directory)) {
    return NotFoundError("Directory does not exist: " + directory);
  }

  std::string canonicalPath = fs::canonical(directory).string();

#if defined(__linux__)
  if (impl_->inotifyFd < 0) {
    return InternalError("inotify not initialized");
  }

  if (impl_->directoryToWd.contains(canonicalPath)) {
    return OkStatus();  // Already watching
  }

  int wd = inotify_add_watch(impl_->inotifyFd, canonicalPath.c_str(),
                             IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO);
  if (wd < 0) {
    return InternalError("Failed to add inotify watch: " +
                         std::string(strerror(errno)));
  }

  impl_->watchDescriptors[wd] = canonicalPath;
  impl_->directoryToWd[canonicalPath] = wd;

  // Also watch subdirectories
  for (const auto& entry : fs::recursive_directory_iterator(canonicalPath)) {
    if (entry.is_directory()) {
      std::string subdir = entry.path().string();
      int subWd =
          inotify_add_watch(impl_->inotifyFd, subdir.c_str(),
                            IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO);
      if (subWd >= 0) {
        impl_->watchDescriptors[subWd] = subdir;
        impl_->directoryToWd[subdir] = subWd;
      }
    }
  }

#elif defined(__APPLE__)
  // macOS FSEvents implementation placeholder
  impl_->watchedDirectories.insert(canonicalPath);
  LOG(WARNING) << "FSEvents hot reload not fully implemented";

#elif defined(_WIN32)
  // Windows ReadDirectoryChangesW implementation placeholder
  LOG(WARNING) << "Windows hot reload not fully implemented";

#else
  LOG(WARNING) << "Hot reload not supported on this platform";
#endif

  active_.store(true);
  LOG(INFO) << "Watching directory for hot reload: " << canonicalPath;
  return OkStatus();
}

void HotReloadManager::unwatchDirectory(const std::string& directory) {
  std::string canonicalPath;
  try {
    canonicalPath = fs::canonical(directory).string();
  } catch (...) {
    canonicalPath = directory;
  }

#if defined(__linux__)
  auto it = impl_->directoryToWd.find(canonicalPath);
  if (it != impl_->directoryToWd.end()) {
    inotify_rm_watch(impl_->inotifyFd, it->second);
    impl_->watchDescriptors.erase(it->second);
    impl_->directoryToWd.erase(it);
  }
#elif defined(__APPLE__)
  impl_->watchedDirectories.erase(canonicalPath);
#elif defined(_WIN32)
  auto it = impl_->directoryHandles.find(canonicalPath);
  if (it != impl_->directoryHandles.end()) {
    CloseHandle(it->second);
    impl_->directoryHandles.erase(it);
  }
#endif

  if (impl_->directoryToWd.empty()) {
    active_.store(false);
  }
}

void HotReloadManager::unwatchAll() {
#if defined(__linux__)
  for (const auto& [wd, _] : impl_->watchDescriptors) {
    inotify_rm_watch(impl_->inotifyFd, wd);
  }
  impl_->watchDescriptors.clear();
  impl_->directoryToWd.clear();
#elif defined(__APPLE__)
  impl_->watchedDirectories.clear();
#elif defined(_WIN32)
  for (auto& [_, handle] : impl_->directoryHandles) {
    CloseHandle(handle);
  }
  impl_->directoryHandles.clear();
#endif
  active_.store(false);
}

void HotReloadManager::onReload(ReloadCallback callback) {
  std::lock_guard lock(mutex_);
  callbacks_.push_back(std::move(callback));
}

void HotReloadManager::pollChanges() {
#if defined(__linux__)
  if (impl_->inotifyFd < 0) {
    return;
  }

  constexpr size_t BUF_SIZE = 4096;
  alignas(inotify_event) char buffer[BUF_SIZE];

  while (true) {
    ssize_t length = read(impl_->inotifyFd, buffer, BUF_SIZE);
    if (length < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;  // No more events
      }
      LOG(WARNING) << "Error reading inotify events: " << strerror(errno);
      break;
    }

    for (ssize_t i = 0; i < length;) {
      auto* event = reinterpret_cast<inotify_event*>(&buffer[i]);

      if (event->len > 0) {
        auto dirIt = impl_->watchDescriptors.find(event->wd);
        if (dirIt != impl_->watchDescriptors.end()) {
          std::string filePath = dirIt->second + "/" + event->name;

          // Only trigger for .lua files
          if (filePath.ends_with(".lua")) {
            notifyChange(filePath);
          }
        }
      }

      i += static_cast<ssize_t>(sizeof(inotify_event) + event->len);
    }
  }
#endif

  // Process pending reloads
  std::vector<std::string> toReload;
  {
    std::lock_guard lock(mutex_);
    toReload.assign(pendingReloads_.begin(), pendingReloads_.end());
  }

  for (const auto& pluginId : toReload) {
    std::lock_guard lock(mutex_);
    for (const auto& callback : callbacks_) {
      callback(pluginId);
    }
  }
}

std::vector<std::string> HotReloadManager::getPendingReloads() {
  std::lock_guard lock(mutex_);
  return std::vector<std::string>{pendingReloads_.begin(),
                                  pendingReloads_.end()};
}

void HotReloadManager::clearPendingReload(const std::string& pluginId) {
  std::lock_guard lock(mutex_);
  pendingReloads_.erase(pluginId);
}

bool HotReloadManager::isActive() const noexcept { return active_.load(); }

void HotReloadManager::registerPluginFile(const std::string& filePath,
                                          const std::string& pluginId) {
  std::lock_guard lock(mutex_);
  std::string canonical;
  try {
    canonical = fs::canonical(filePath).string();
  } catch (...) {
    canonical = filePath;
  }
  fileToPlugin_[canonical] = pluginId;
}

void HotReloadManager::unregisterPluginFiles(const std::string& pluginId) {
  std::lock_guard lock(mutex_);
  std::erase_if(fileToPlugin_, [&pluginId](const auto& pair) {
    return pair.second == pluginId;
  });
}

void HotReloadManager::notifyChange(const std::string& filePath) {
  std::lock_guard lock(mutex_);

  std::string canonical;
  try {
    canonical = fs::canonical(filePath).string();
  } catch (...) {
    canonical = filePath;
  }

  auto it = fileToPlugin_.find(canonical);
  if (it != fileToPlugin_.end()) {
    pendingReloads_.insert(it->second);
    LOG(INFO) << "File changed, scheduling reload: " << it->second;
  } else {
    // Try to find the plugin by directory
    for (const auto& [file, plugin] : fileToPlugin_) {
      fs::path pluginDir = fs::path(file).parent_path();
      if (canonical.starts_with(pluginDir.string())) {
        pendingReloads_.insert(plugin);
        LOG(INFO) << "File changed in plugin directory, scheduling reload: "
                  << plugin;
        break;
      }
    }
  }
}

}  // namespace uinta
