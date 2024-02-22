#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_

#include "uinta/engine_stage.h"
#include "uinta/input.h"

namespace uinta {

class EngineState {
  using EngineStateFlags = bitflag8;

 public:
  EngineState() noexcept = default;
  ~EngineState() noexcept = default;

  EngineState(const EngineState&) noexcept = delete;
  EngineState& operator=(const EngineState&) noexcept = delete;
  EngineState(EngineState&& other) noexcept = delete;
  EngineState& operator=(EngineState&& other) noexcept = delete;

  void advance(f32 runtime) noexcept {
    tick_++;
    delta_ = runtime - runtime_;
    frameDelta_ += delta_;
    runtime_ = runtime;
  }

  u32 tick() const noexcept { return tick_; }

  u32 frame() const noexcept { return frame_; }

  f32 delta() const noexcept { return delta_; }

  f32 frameDelta() const noexcept { return frameDelta_; }

  void resetFrameDelta() noexcept { frameDelta_ = 0; }

  f32 runtime() const noexcept { return runtime_; }

  bool isNewFrame() const noexcept { return flags_ & NewFrameMask; }

  void newFrame(bool v) noexcept {
    flags_ &= ~NewFrameMask;
    if (v) {
      flags_ |= NewFrameMask;
      frame_++;
    }
  }

  bool isClosing() const noexcept { return flags_ & ClosingMask; }

  void setClosing() noexcept { flags_ |= ClosingMask; }

  EngineStage stage() const noexcept {
    return static_cast<EngineStage>((flags_ & StageMask) >> StageShift);
  }

  void stage(EngineStage stage) noexcept {
    flags_ &= ~StageMask;
    flags_ |= static_cast<EngineStateFlags>(stage) << StageShift;
  }

  const Input& input() const noexcept { return input_; }

  Input& input() noexcept { return input_; }

  bool isKeyDown(InputKey key, InputMod mask = 0) const noexcept {
    return input_.isKeyDown(key, mask);
  }

  bool isKeyPressed(InputKey key, InputMod mask = 0) const noexcept {
    return input_.isKeyPressed(key, mask);
  }

  bool isKeyReleased(InputKey key, InputMod mask = 0) const noexcept {
    return input_.isKeyReleased(key, mask);
  }

  bool isKeyRepeated(InputKey key, InputMod mask = 0) const noexcept {
    return input_.isKeyRepeated(key, mask);
  }

  bool isMouseDown(MouseButton button, InputMod mask = 0) const noexcept {
    return input_.isMouseDown(button, mask);
  }

  bool isMousePressed(MouseButton button, InputMod mask = 0) const noexcept {
    return input_.isMousePressed(button, mask);
  }

  bool isMouseReleased(MouseButton button, InputMod mask = 0) const noexcept {
    return input_.isMouseReleased(button, mask);
  }

 private:
  static constexpr EngineStateFlags NewFrameMask = 1 << 0;
  static constexpr EngineStateFlags ClosingMask = 1 << 1;

  static constexpr EngineStateFlags StageShift = 2;
  static constexpr EngineStateFlags StageMask = 0x7 << StageShift;

  Input input_;
  f32 delta_ = 0;
  f32 frameDelta_ = 0;
  u32 frame_ = 0;
  f32 runtime_ = 0;
  u32 tick_ = 0;
  EngineStateFlags flags_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_STATE_H_
