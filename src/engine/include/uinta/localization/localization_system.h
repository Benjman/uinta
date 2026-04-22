#ifndef SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_

#include <string>
#include <unordered_map>

#include "uinta/localization/locale.h"
#include "uinta/localization/localization.h"

namespace uinta {

// Engine-owned, service-discoverable subsystem responsible for loading and
// serving translated strings from a per-locale YAML resource file.
//
// The resource file is resolved via `FindFile` from the path
// `localization/<toString(locale)>.yml`. The file must exist and be parseable
// at construction time; otherwise the constructor will assert.
//
// Typical usage:
//   const auto* localization = engine->service<LocalizationSystem>();
//   std::string s = localization->getString(Localization::HelloLocalization);
class LocalizationSystem {
 public:
  LocalizationSystem(Locale locale) noexcept;
  ~LocalizationSystem() noexcept = default;

  LocalizationSystem(const LocalizationSystem&) = delete;
  LocalizationSystem& operator=(const LocalizationSystem&) = delete;
  LocalizationSystem(LocalizationSystem&&) = delete;
  LocalizationSystem& operator=(LocalizationSystem&&) = delete;

  [[nodiscard]] Locale locale() const noexcept { return locale_; }

  // Returns the translated value for the given key. If the key is not present
  // in the loaded resource file, logs a warning and returns the key's YAML
  // identifier (from `toKey`) as a diagnostic fallback.
  [[nodiscard]] std::string getString(Localization key) const noexcept;

 private:
  Locale locale_;
  std::unordered_map<Localization, std::string> strings_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_
