#ifndef UINTA_FLAGS_HPP
#define UINTA_FLAGS_HPP

namespace uinta {

// TODO maybe have different sizes of flags: flags8_t, flags16_t, flags32_t...

using flag_t = unsigned int;
using flags_t = unsigned int;

inline bool isFlagSet(const flag_t mask, const flags_t flags) {
  return flags & mask;
}

inline void setFlag(const flag_t mask, const bool state, flags_t& flags) {
  flags &= ~mask;
  if (state) flags |= mask;
}

}  // namespace uinta

#endif  // UINTA_RUNNER_FLAGS_HPP
