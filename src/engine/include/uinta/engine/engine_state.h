#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_

#include <atomic>

#include "uinta/flags.h"
#include "uinta/types.h"

namespace uinta {

class EngineState {
 public:
  EngineState() noexcept = default;
  ~EngineState() noexcept = default;

  void update(time_t runtime, count_t tickIncrement = 1) noexcept {
    tick_ += tickIncrement;
    lastRuntime_ = runtime_;
    if (isNewFrame()) {
      frame_++;
      lastFrameRuntime_ = runtime_;
    }
    runtime_ = runtime;
  }

  auto tick() const noexcept { return tick_; }

  auto frame() const noexcept { return frame_; }

  auto delta() const noexcept { return runtime_ - lastRuntime_; }

  auto frameDelta() const noexcept { return runtime_ - lastFrameRuntime_; }

  auto runtime() const noexcept { return runtime_; }

  void isNewFrame(bool v) noexcept { flags_.isNewFrame(v); }

  bool isNewFrame() const noexcept { return flags_.isNewFrame(); }

  bool isClosing() const noexcept { return flags_.isClosing(); }

  void isClosing(bool v) noexcept { flags_.isClosing(v); }

 private:
  struct Flags final {
    using value_type = u8;

    AtomicFlagsOperations(0);

    bool isNewFrame() const noexcept { return flags_ & NewFrameMask; }
    void isNewFrame(bool v) noexcept {
      flags_ &= ~NewFrameMask;
      if (v) flags_ |= NewFrameMask;
    }

    bool isClosing() const noexcept { return flags_ & ClosingMask; }
    void isClosing(bool v) noexcept {
      flags_ &= ~ClosingMask;
      if (v) flags_ |= ClosingMask;
    }

   private:
    static constexpr value_type NewFrameMask = 1 << 0;
    static constexpr value_type ClosingMask = 1 << 1;

    std::atomic<value_type> flags_;
  } flags_;

  time_t lastRuntime_ = 0;
  time_t lastFrameRuntime_ = -1;
  time_t runtime_ = 0;
  count_t frame_ = 0;
  count_t tick_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STATE_H_
