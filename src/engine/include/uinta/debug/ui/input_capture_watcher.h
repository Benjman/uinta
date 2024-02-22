#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_CAPTURE_WATCHER_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_CAPTURE_WATCHER_H_

#include <imgui.h>

#include "uinta/engine_state.h"
#include "uinta/system.h"

namespace uinta {

class InputCaptureWatcher : public System {
 public:
  explicit InputCaptureWatcher(Input* input) noexcept : input_(input) {}

  void onPreTick(const EngineState&) noexcept override {
    auto& io_ = ImGui::GetIO();
    if (io_.WantCaptureMouse) input_->resetMouse();
    if (io_.WantCaptureKeyboard) input_->resetKeyboard();
  }

 private:
  Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_CAPTURE_WATCHER_H_
