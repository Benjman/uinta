#ifndef UINTA_STRING_UTILS_FWD_H
#define UINTA_STRING_UTILS_FWD_H

namespace uinta {

// Returns true if `keys` contains `key`
//   `keys` must be a null terminated array.
inline bool containsKey(const char* const key, const char** const keys);

}  // namespace uinta

#endif  // UINTA_STRING_UTILS_FWD_H
