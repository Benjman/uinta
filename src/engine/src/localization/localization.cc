#include "uinta/localization/localization.h"

#include <absl/log/log.h>

#include <optional>
#include <string_view>

namespace uinta {

std::string_view toKey(Localization key) noexcept {
  switch (key) {
    case Localization::HelloLocalization:
      return "hello_localization";
    default:
      LOG(FATAL) << "Invalid localization!";
  }
}

std::optional<Localization> fromKey(std::string_view yamlKey) noexcept {
  if (yamlKey == toKey(Localization::HelloLocalization)) {
    return Localization::HelloLocalization;
  }
  LOG(FATAL) << "Invalid localization key!";
}

}  // namespace uinta
