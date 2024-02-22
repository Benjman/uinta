#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_STATE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_STATE_UI_H_

#include <imgui.h>

#include "absl/time/time.h"
#include "uinta/scene.h"

namespace uinta {

class EngineStateUi {
 public:
  EngineStateUi() = default;
  EngineStateUi(const EngineStateUi&) noexcept = delete;
  EngineStateUi& operator=(const EngineStateUi&) noexcept = delete;
  EngineStateUi(EngineStateUi&&) noexcept = delete;
  EngineStateUi& operator=(EngineStateUi&&) noexcept = delete;

  void render(const EngineState& state) noexcept {
    absl::Duration duration = absl::Seconds(std::round(state.runtime()));
    ImGui::Text("Runtime: %s", absl::FormatDuration(duration).c_str());
    ImGui::Text("Delta: %f", state.delta());
    ImGui::Text("Tick: %lu", state.tick());
    ImGui::Text("Ticks/sec: %lu",
                static_cast<u64>(state.tick() / state.runtime()));
    if (state.frame())
      ImGui::Text("Ticks/frame: %lu",
                  static_cast<u64>(state.tick() / state.frame()));
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_STATE_UI_H_
