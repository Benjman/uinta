#ifndef UINTA_RUNNER_FLAGS_HPP
#define UINTA_RUNNER_FLAGS_HPP

#include <uinta/runner/fwd.hpp>

namespace uinta {

const flag_t RUNNER_FLAG_CAMERA = 1 << 0;

inline bool isCameraEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_CAMERA, flags);
}

}  // namespace uinta

#endif  // UINTA_RUNNER_FLAGS_HPP
