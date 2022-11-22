#ifndef UINTA_RUNNER_FLAGS_HPP
#define UINTA_RUNNER_FLAGS_HPP

#include <uinta/runner/fwd.hpp>

namespace uinta {

const flag_t RUNNER_FLAG_CAMERA = 1 << 0;
const flag_t RUNNER_FLAG_GRID = 1 << 1;

inline bool isCameraEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_CAMERA, flags);
}

inline bool isGridEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_GRID, flags);
}

}  // namespace uinta

#endif  // UINTA_RUNNER_FLAGS_HPP
