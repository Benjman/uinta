#ifndef UINTA_RUNNER_RUNNER_STATE_HPP
#define UINTA_RUNNER_RUNNER_STATE_HPP

#include "./display.hpp"
#include "uinta/input.hpp"

namespace uinta {

struct RunnerState final {
  /// tick cycle iteration
  unsigned int tick;

  /// time in seconds since last tick cycle
  float delta;

  /// time in secinds the runner has been ticking
  float runtime;

  InputState input;
  Display display;

  RunnerState() {
    tick = 0;
    delta = 0.0;
    runtime = 0.0;
    input = InputState();
    display = Display();
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_STATE_HPP
