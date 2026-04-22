#ifndef SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_

#include <string>
#include <unordered_map>

#include "uinta/localization/locale.h"
#include "uinta/localization/localization.h"

namespace uinta {

class LocalizationSystem {
 public:
  LocalizationSystem(Locale locale) noexcept;
  ~LocalizationSystem() noexcept = default;

  LocalizationSystem(const LocalizationSystem&) = delete;
  LocalizationSystem& operator=(const LocalizationSystem&) = delete;
  LocalizationSystem(LocalizationSystem&&) = delete;
  LocalizationSystem& operator=(LocalizationSystem&&) = delete;

  [[nodiscard]] Locale locale() const noexcept { return locale_; }

  [[nodiscard]] std::string getString(Localization key) const noexcept;

 private:
  Locale locale_;
  std::unordered_map<Localization, std::string> strings_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALIZATION_SYSTEM_H_
