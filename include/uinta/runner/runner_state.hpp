#ifndef UINTA_RUNNER_RUNNER_STATE_HPP
#define UINTA_RUNNER_RUNNER_STATE_HPP

#include <uinta/utils/types.h>

#include <uinta/runner/fwd.hpp>

namespace uinta {

struct RunnerState final {
  /// tick cycle
  uint tick = 0;

  /// time in seconds since last tick cycle
  float delta = 0;

  /// time in seconds the runner has been ticking
  float runtime = 0;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_STATE_HPP
