#ifndef SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_
#define SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_

#include "uinta/engine/engine.h"

namespace uinta {

class InputFrameGuard {
 public:
  explicit InputFrameGuard(Engine* engine) noexcept : input_(engine->input()) {
    if (auto status = engine->platform()->pollEvents(); !status.ok()) {
      engine->setStatusError(status);
    }
  }

  ~InputFrameGuard() noexcept { input_->reset(); }

  InputFrameGuard(const InputFrameGuard&) noexcept = delete;
  InputFrameGuard& operator=(const InputFrameGuard&) noexcept = delete;
  InputFrameGuard(InputFrameGuard&&) noexcept = delete;
  InputFrameGuard& operator=(InputFrameGuard&&) noexcept = delete;

 private:
  Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_INPUT_FRAME_GUARD_H_
