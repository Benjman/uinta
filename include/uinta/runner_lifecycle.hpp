#ifndef UINTA_RUNNER_LIFECYCLE_HPP
#define UINTA_RUNNER_LIFECYCLE_HPP

#include <uinta/fwd.hpp>

namespace uinta {

class RunnerLifecycle {
 public:
  virtual uinta_error_code init() = 0;

  virtual void pre_tick(const RunnerState& state, const InputState& input) {
  }

  virtual void tick(const RunnerState& state, const InputState& input) {
  }

  virtual void post_tick(const RunnerState& state, const InputState& input) {
  }

  virtual void pre_render(const RunnerState& state) {
  }

  virtual void render(const RunnerState& state) {
  }

  virtual void post_render(const RunnerState& state) {
  }

  virtual void shutdown() {
  }
};

};  // namespace uinta

#endif  // UINTA_RUNNER_LIFECYCLE_HPP
