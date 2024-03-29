#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_

#include "uinta/types.h"

namespace uinta {

class Platform;

class EngineState {
  using EngineStateFlags = bitflag8;

 public:
  EngineState() noexcept = default;
  ~EngineState() noexcept = default;

  EngineState(const EngineState&) noexcept = delete;
  EngineState& operator=(const EngineState&) noexcept = delete;

  EngineState(EngineState&& other) noexcept
      : delta_(other.delta_),
        frame_(other.frame_),
        runtime_(other.runtime_),
        tick_(other.tick_),
        flags_(other.flags_) {
    other.delta_ = 0;
    other.frame_ = 0;
    other.runtime_ = 0;
    other.tick_ = 0;
    other.flags_ = 0;
  }

  EngineState& operator=(EngineState&& other) noexcept {
    if (this != &other) {
      delta_ = other.delta_;
      frame_ = other.frame_;
      runtime_ = other.runtime_;
      tick_ = other.tick_;
      flags_ = other.flags_;

      other.delta_ = 0;
      other.frame_ = 0;
      other.runtime_ = 0;
      other.tick_ = 0;
      other.flags_ = 0;
    }
    return *this;
  }

  void advance(f32 runtime) noexcept {
    tick_++;
    delta_ = runtime - runtime_;
    runtime_ = runtime;
  }

  u32 tick() const noexcept { return tick_; }

  u32 frame() const noexcept { return frame_; }

  f32 delta() const noexcept { return delta_; }

  f32 runtime() const noexcept { return runtime_; }

  bool isNewFrame() const noexcept { return flags_ & NewFrameMask; }

  void isNewFrame(bool v) noexcept {
    flags_ &= ~NewFrameMask;
    if (v) flags_ |= NewFrameMask;
  }

  bool isClosing() const noexcept { return flags_ & ClosingMask; }

  void setClosing() noexcept { flags_ |= ClosingMask; }

 private:
  static constexpr EngineStateFlags NewFrameMask = 1 << 0;
  static constexpr EngineStateFlags ClosingMask = 1 << 1;

  f32 delta_ = 0;
  u32 frame_ = 0;
  f32 runtime_ = 0;
  u32 tick_ = 0;
  EngineStateFlags flags_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_
