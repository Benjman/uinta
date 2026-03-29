#include "uinta/lua/api_version.h"

#include <array>
#include <charconv>
#include <string_view>

namespace uinta {

ApiVersion ApiVersion::parse(const std::string& versionStr) noexcept {
  ApiVersion version{.major = 0, .minor = 0, .patch = 0};

  std::string_view sv(versionStr);
  std::array<u8*, 3> parts{&version.major, &version.minor, &version.patch};
  size_t partIndex = 0;

  size_t start = 0;
  for (size_t i = 0; i <= sv.size() && partIndex < 3; ++i) {
    if (i == sv.size() || sv[i] == '.') {
      if (i > start) {
        std::string_view part = sv.substr(start, i - start);
        int value = 0;
        auto result =
            std::from_chars(part.data(), part.data() + part.size(), value);
        if (result.ec == std::errc{} && value >= 0 && value <= 255) {
          *parts[partIndex] = static_cast<u8>(value);
        }
      }
      start = i + 1;
      ++partIndex;
    }
  }

  return version;
}

std::string ApiVersion::toString() const noexcept {
  return std::to_string(major) + "." + std::to_string(minor) + "." +
         std::to_string(patch);
}

}  // namespace uinta
