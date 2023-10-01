#ifndef UINTA_RUNNER_RUNNER_STATE_HPP
#define UINTA_RUNNER_RUNNER_STATE_HPP

#include <uinta/types.h>

namespace uinta {

struct RunnerState final {
  /// tick cycle
  u32 tick = 0;

  /// time in seconds since last tick cycle
  f32 delta = 0;

  /// time in seconds the runner has been ticking
  f32 runtime = 0;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_STATE_HPP
