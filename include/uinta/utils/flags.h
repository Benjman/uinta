#ifndef UINTA_FLAGS_H
#define UINTA_FLAGS_H

#include <uinta/utils/types.h>

using flag_t = uint;
using flags_t = uint;

inline bool isFlagSet(const flag_t mask, const flags_t flags) {
  return flags & mask;
}

inline void setFlag(const flag_t mask, const bool state, flags_t& flags) {
  flags &= ~mask;
  if (state) flags |= mask;
}

#endif  // UINTA_RUNNER_FLAGS_H
