#ifndef UINTA_STRING_UTILS_FWD_HPP
#define UINTA_STRING_UTILS_FWD_HPP

namespace uinta {

// Returns true if `keys` contains `key`
//   `keys` must be a null terminated array.
inline bool containsKey(const char* const key, const char** const keys);

}  // namespace uinta

#endif  // UINTA_STRING_UTILS_FWD_HPP
