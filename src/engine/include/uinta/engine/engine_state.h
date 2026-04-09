#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_

#include <array>
#include <atomic>

#include "uinta/engine/engine_stage.h"
#include "uinta/flags.h"
#include "uinta/types.h"

namespace uinta {

class EngineState {
 public:
  EngineState() noexcept = default;
  ~EngineState() noexcept = default;

  void updateRuntime(time_t runtime) noexcept {
    delta_ = runtime - runtime_;
    runtime_ = runtime;
  }

  time_t updateStageDelta(EngineStage stage, time_t currentRuntime) noexcept {
    auto index = static_cast<size_t>(stage);
    auto delta = currentRuntime - stageRuntimes_[index];
    stageRuntimes_[index] = currentRuntime;
    return delta;
  }

  void addTick(i32 count = 1) noexcept { tickCount_ += count; }

  void addFrame(i32 count = 1) noexcept { frameCount_ += count; }

  [[nodiscard]] auto tick() const noexcept { return tickCount_; }

  [[nodiscard]] auto frame() const noexcept { return frameCount_; }

  [[nodiscard]] auto delta() const noexcept { return delta_; }

  [[nodiscard]] auto runtime() const noexcept { return runtime_; }

  [[nodiscard]] bool isClosing() const noexcept { return flags_.isClosing(); }

  void isClosing(bool v) noexcept { flags_.isClosing(v); }

  [[nodiscard]] bool isFixedTickRate() const noexcept {
    return flags_.isFixedTickRate();
  }

  void isFixedTickRate(bool v) noexcept { flags_.isFixedTickRate(v); }

 private:
  struct Flags final {
    using value_type = u8;

    AtomicFlagsOperations(FixedTickRateMask);

    [[nodiscard]] bool isClosing() const noexcept {
      return (flags_ & ClosingMask) != 0;
    }
    void isClosing(bool v) noexcept {
      flags_ &= ~ClosingMask;
      if (v) {
        flags_ |= ClosingMask;
      }
    }

    [[nodiscard]] bool isFixedTickRate() const noexcept {
      return (flags_ & FixedTickRateMask) != 0;
    }

    void isFixedTickRate(bool v) noexcept {
      flags_ &= ~FixedTickRateMask;
      if (v) {
        flags_ |= FixedTickRateMask;
      }
    }

   private:
    static constexpr value_type ClosingMask = 1 << 0;
    static constexpr value_type FixedTickRateMask = 1 << 2;

    std::atomic<value_type> flags_;
  } flags_;

  time_t runtime_ = 0;
  time_t delta_ = 0;
  std::array<time_t, 6> stageRuntimes_ = {0, 0, 0, 0, 0, 0};
  count_t frameCount_ = 0;
  count_t tickCount_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_
