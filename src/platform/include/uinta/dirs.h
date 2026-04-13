#ifndef SRC_PLATFORM_INCLUDE_UINTA_DIRS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DIRS_H_

#include <cstdlib>
#include <filesystem>
#include <optional>

#if defined(_WIN32)
#include <shlobj.h>
#include <windows.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

#include <absl/log/log.h>

#include "uinta/types.h"

namespace uinta {

enum class DirType : u8 {
  CacheDir,
  ConfigDir,
  DataDir,
  RuntimeDir,
  StateDir,
};

namespace _ {

inline std::string SafeGetenv(const char* name) noexcept {
  const char* val = std::getenv(name);
  return (val != nullptr) ? val : std::string{};
}

inline const std::string& GetHome() noexcept {
  static const std::string home = []() -> std::string {
#if defined(_WIN32)
    return SafeGetenv("USERPROFILE");
#else
    auto h = SafeGetenv("HOME");
    if (h.empty()) {
      if (const passwd* pw = getpwuid(getuid())) {
        h = pw->pw_dir;
      }
    }
    return h;
#endif
  }();
  return home;
}

#if defined(_WIN32)
inline std::filesystem::path GetKnownFolder(REFKNOWNFOLDERID id) noexcept {
  PWSTR path = nullptr;
  std::filesystem::path result;
  if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &path))) {
    result = path;
  }
  if (path) CoTaskMemFree(path);
  return result;
}
#endif

}  // namespace _

template <DirType T>
std::optional<std::filesystem::path> GetDir() {
  if (_::GetHome().empty()) {
    LOG(ERROR) << "Failed to find a `$HOME` value.";
    return std::nullopt;
  }

#if defined(__APPLE__)
  if constexpr (T == DirType::CacheDir) {
    static const std::filesystem::path dir =
        std::filesystem::path(_::GetHome()) / "Library" / "Caches";
    return dir;
  } else if constexpr (T == DirType::ConfigDir) {
    static const std::filesystem::path dir =
        std::filesystem::path(_::GetHome()) / "Library" / "Preferences";
    return dir;
  } else if constexpr (T == DirType::DataDir) {
    static const std::filesystem::path dir =
        std::filesystem::path(_::GetHome()) / "Library" / "Application Support";
    return dir;
  } else if constexpr (T == DirType::RuntimeDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("TMPDIR"); !env.empty()) return env;
      return "/tmp";
    }();
    return dir;
  } else if constexpr (T == DirType::StateDir) {
    static const std::filesystem::path dir =
        std::filesystem::path(_::GetHome()) / "Library" / "Application Support";
    return dir;
  } else {
    static_assert(false, "Unhandled DirType");
  }
#elif defined(__linux__)
  if constexpr (T == DirType::CacheDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("XDG_CACHE_HOME"); !env.empty()) {
        return env;
      }
      return std::filesystem::path(_::GetHome()) / ".cache";
    }();
    return dir;
  } else if constexpr (T == DirType::ConfigDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("XDG_CONFIG_HOME"); !env.empty()) {
        return env;
      }
      return std::filesystem::path(_::GetHome()) / ".config";
    }();
    return dir;
  } else if constexpr (T == DirType::DataDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("XDG_DATA_HOME"); !env.empty()) {
        return env;
      }
      return std::filesystem::path(_::GetHome()) / ".local" / "share";
    }();
    return dir;
  } else if constexpr (T == DirType::RuntimeDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("XDG_RUNTIME_DIR"); !env.empty()) {
        return env;
      }
      return "/tmp";
    }();
    return dir;
  } else if constexpr (T == DirType::StateDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("XDG_STATE_HOME"); !env.empty()) {
        return env;
      }
      return std::filesystem::path(_::GetHome()) / ".local" / "state";
    }();
    return dir;
  } else {
    static_assert(false, "Unhandled DirType");
  }
#elif defined(_WIN32)
  if constexpr (T == DirType::CacheDir) {
    static const std::filesystem::path dir =
        _::GetKnownFolder(FOLDERID_LocalAppData);
    return dir;
  } else if constexpr (T == DirType::ConfigDir) {
    static const std::filesystem::path dir =
        _::GetKnownFolder(FOLDERID_RoamingAppData);
    return dir;
  } else if constexpr (T == DirType::DataDir) {
    static const std::filesystem::path dir =
        _::GetKnownFolder(FOLDERID_LocalAppData);
    return dir;
  } else if constexpr (T == DirType::RuntimeDir) {
    static const std::filesystem::path dir = []() -> std::filesystem::path {
      if (auto env = _::SafeGetenv("TEMP"); !env.empty()) return env;
      return _::GetKnownFolder(FOLDERID_LocalAppData) / "Temp";
    }();
    return dir;
  } else if constexpr (T == DirType::StateDir) {
    static const std::filesystem::path dir =
        _::GetKnownFolder(FOLDERID_LocalAppData);
    return dir;
  } else {
    static_assert(false, "Unhandled DirType");
  }
#else
  static_assert(false, "Unsupported platform");
#endif
}

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DIRS_H_
