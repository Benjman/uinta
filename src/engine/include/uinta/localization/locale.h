#ifndef SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALE_H_
#define SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALE_H_

#include <string>

#include "uinta/types.h"

namespace uinta {

enum class Locale : u8 {
  Unknown,
  EnUs,
};

inline std::string toString(Locale locale) noexcept {
  switch (locale) {
    case Locale::EnUs:
      return "en-us";
    default:
      return "unknown";
  }
}

inline Locale toLocale(const std::string& locale) noexcept {
  if ("en-us" == locale) {
    return Locale::EnUs;
  }
  return Locale::Unknown;
}

// Returns true when `locale` is `Locale::Unknown`, enabling the
// `if (!locale) { ... }` idiom (Protobuf-style "zero value is falsy").
inline bool operator!(Locale locale) noexcept {
  return locale == Locale::Unknown;
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LOCALIZATION_LOCALE_H_
