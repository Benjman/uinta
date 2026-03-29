#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_API_VERSION_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_API_VERSION_H_

#include <string>

#include "uinta/types.h"

namespace uinta {

/// API version for plugin compatibility checking.
/// Plugins declare the API version they require, and the runtime checks
/// compatibility before loading.
struct ApiVersion {
  u8 major;  ///< Breaking changes
  u8 minor;  ///< New features (backward compatible)
  u8 patch;  ///< Bug fixes

  /// Check if this version is compatible with a required version.
  /// Compatible means: same major version, and minor >= required minor.
  [[nodiscard]] constexpr bool isCompatible(
      const ApiVersion& required) const noexcept {
    return major == required.major && minor >= required.minor;
  }

  /// Parse a version string in the format "major.minor.patch".
  /// Returns a default {0,0,0} version if parsing fails.
  [[nodiscard]] static ApiVersion parse(const std::string& versionStr) noexcept;

  /// Convert to string representation.
  [[nodiscard]] std::string toString() const noexcept;

  [[nodiscard]] constexpr bool operator==(
      const ApiVersion& other) const noexcept {
    return major == other.major && minor == other.minor && patch == other.patch;
  }

  [[nodiscard]] constexpr bool operator!=(
      const ApiVersion& other) const noexcept {
    return !(*this == other);
  }

  [[nodiscard]] constexpr bool operator<(
      const ApiVersion& other) const noexcept {
    if (major != other.major) {
      return major < other.major;
    }
    if (minor != other.minor) {
      return minor < other.minor;
    }
    return patch < other.patch;
  }
};

/// Current API version provided by the engine.
inline constexpr ApiVersion CURRENT_LUA_API_VERSION = {1, 0, 0};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_API_VERSION_H_
