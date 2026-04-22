#ifndef SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_
#define SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_

#include <optional>
#include <string_view>

#include "uinta/types.h"

namespace uinta {

enum class Localization : u16 {  // NOLINT
  HelloLocalization,
};

std::string_view toKey(Localization key) noexcept;

std::optional<Localization> fromKey(std::string_view yamlKey) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_H_
