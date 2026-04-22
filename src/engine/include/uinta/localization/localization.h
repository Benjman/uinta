#ifndef SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_
#define SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_

#include <optional>
#include <string_view>

#include "uinta/types.h"

namespace uinta {

// Registry of all localizable strings in the engine. Add new keys here and
// provide a translation for each supported Locale in the corresponding YAML
// resource file (e.g. `res/localization/en-us.yml`).
enum class Localization : u16 {  // NOLINT
  HelloLocalization,
};

// Maps a `Localization` enum value to the YAML key used in the resource file.
std::string_view toKey(Localization key) noexcept;

// Inverse of `toKey`. Returns the `Localization` value corresponding to the
// given YAML key, or `std::nullopt` if no such key is registered.
std::optional<Localization> fromKey(std::string_view yamlKey) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_
