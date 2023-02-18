#ifndef UINTA_STRING_UTILS_H
#define UINTA_STRING_UTILS_H

#include <uinta/utils/string_utils_fwd.h>

#include <cstring>

#ifdef UINTA_DEBUG
#include <uinta/logging.hpp>
#endif

// TODO remove namespacing in this file
namespace uinta {

inline bool containsKey(const char* const key, const char** const keys) {
  for (int i = 0; keys[i]; i++) {
    if (strcmp(key, keys[i]) == 0) return true;
#ifdef UINTA_DEBUG
    if (i >= 100 && i % 100 == 0) SPDLOG_WARN("`containsKey()` has looped {} times without encountering a nullptr.", i);
#endif
  }
  return false;
}

}  // namespace uinta

#endif  // UINTA_STRING_UTILS_H
