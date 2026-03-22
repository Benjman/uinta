#ifndef SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_
#define SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_

#include "uinta/engine/engine.h"

namespace uinta {

class InputFrameGuard {
 public:
  explicit InputFrameGuard(Engine* engine, time_t dt) noexcept : inputSystem_(engine->service<InputSystem>()) {
    assert(inputSystem_ && "InputFrameGuard: InputSystem must be registered as a service!");
    if (auto status = engine->platform()->pollEvents(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    inputSystem_->update(dt);
  }

  ~InputFrameGuard() noexcept { inputSystem_->endFrame(); }

  InputFrameGuard(const InputFrameGuard&) noexcept = delete;
  InputFrameGuard& operator=(const InputFrameGuard&) noexcept = delete;
  InputFrameGuard(InputFrameGuard&&) noexcept = delete;
  InputFrameGuard& operator=(InputFrameGuard&&) noexcept = delete;

 private:
  InputSystem* inputSystem_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_
