#ifndef UINTA_RUNNER_FLAGS_HPP
#define UINTA_RUNNER_FLAGS_HPP

#include <uinta/flags.hpp>

namespace uinta {

const flag_t RUNNER_FLAG_GRID = 0x1;
const flag_t RUNNER_FLAG_CAMERA = 0x2;
const flag_t RUNNER_FLAG_RENDERING = 0x4;

inline bool isCameraEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_CAMERA, flags);
}

inline bool isGridEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_GRID, flags);
}

inline bool isRenderingEnabled(flags_t flags) {
  return isFlagSet(flags, RUNNER_FLAG_RENDERING);
}

}  // namespace uinta

#endif  // UINTA_RUNNER_FLAGS_HPP
