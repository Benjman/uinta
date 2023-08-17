#ifndef UINTA_FLAGS_H
#define UINTA_FLAGS_H

#include <uinta/types.h>

namespace uinta {

using flag_t = u32;
using flags_t = u32;

inline bool isFlagSet(const flag_t mask, const flags_t flags) {
  return (flags & mask) != 0;
}

inline void setFlag(const flag_t mask, const bool state, flags_t& flags) {
  flags &= ~mask;
  if (state) flags |= mask;
}

}  // namespace uinta

#endif  // UINTA_FLAGS_H
