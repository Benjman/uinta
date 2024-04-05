#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_

#include <imgui.h>

#include <algorithm>
#include <string>
#include <vector>

#include "absl/time/time.h"
#include "uinta/debug/ui/utils.h"
#include "uinta/engine/engine.h"
#include "uinta/math/running_average.h"

namespace uinta {

struct EngineUiInfo final {
  Engine* engine;

  using DeltaAvg = RunningAverage<f32, 100>;
  DeltaAvg delta;

  using FpsCounterAvg = RunningAverage<f32, 3>;
  FpsCounterAvg fps;

  using TickAvg = RunningAverage<f32, 100>;
  TickAvg tick;

  using RenderAvg = RunningAverage<f32, 100>;
  RenderAvg render;
};

inline void RenderEngineUi(const EngineUiInfo& info) noexcept {
  if (!ImGui::TreeNode("Engine")) return;

  auto* engine = info.engine;
  auto& fps = info.fps;
  auto& tick = info.tick;
  auto& render = info.render;
  auto isFixedTickRate = engine->state().isFixedTickRate();

  auto& state = engine->state();
  auto size = std::max(tick.size(), render.size());
  std::vector<f32> container(size);

  ImGui::Text("%s %s", "Runtime",
              formatDuration(absl::Seconds(state.runtime()),
                             OmitMiilliseconds | OmitMicroseconds)
                  .c_str());

  ImGui::SameLine(UiHalfWidth);
  ImGui::Text("%s %2.f", "FPS", fps.getAverage());

  ImGui::Text("%s %lu", "Frame", state.frame());

  ImGui::SameLine(UiHalfWidth);
  ImGui::Text("%s %lu", "Tick", state.tick());

  if (ImGui::Checkbox("Sync ticks and frames", &isFixedTickRate)) {
    engine->state().isFixedTickRate(isFixedTickRate);
  }

  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text("Tick %s",
              formatDuration(absl::Seconds(tick.getAverage())).c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  std::copy(tick.begin(), tick.end(), container.begin());
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::PlotLines("##Tick", container.data(), tick.size());
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text("Render %s",
              formatDuration(absl::Seconds(render.getAverage())).c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  std::copy(render.begin(), render.end(), container.begin());
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::PlotLines("##Render", container.data(), render.size());
  ImGui::PopItemWidth();

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_
