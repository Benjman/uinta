#ifndef UINTA_RUNNER_FLAGS_HPP
#define UINTA_RUNNER_FLAGS_HPP

#include <uinta/flags.hpp>

namespace uinta {

bool isCameraEnabled(flags_t flags);
bool isGridEnabled(flags_t flags);

const flag_t RUNNER_FLAG_GRID = 0x1;
const flag_t RUNNER_FLAG_CAMERA = 0x2;

inline bool isCameraEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_CAMERA, flags);
}

inline bool isGridEnabled(flags_t flags) {
  return isFlagSet(RUNNER_FLAG_GRID, flags);
}

}  // namespace uinta

#endif  // UINTA_RUNNER_FLAGS_HPP
