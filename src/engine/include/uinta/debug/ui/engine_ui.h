#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_

#include <imgui.h>

#include <algorithm>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
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

  using FrameDeltaAvg = RunningAverage<f32, 100>;
  FrameDeltaAvg frameDelta;
};

inline void RenderEngineUi(const EngineUiInfo& info) noexcept {
  if (!ImGui::TreeNode("Engine")) return;

  auto* engine = info.engine;
  auto& delta = info.delta;
  auto& fps = info.fps;
  auto& tick = info.tick;
  auto& render = info.render;
  auto& frameDelta = info.frameDelta;

  auto& state = engine->state();
  auto size = std::max(delta.size(), frameDelta.size());
  size = std::max(size, delta.size());
  size = std::max(size, frameDelta.size());
  size = std::max(size, tick.size());
  size = std::max(size, render.size());
  std::vector<f32> container(size);
  auto flags = engine->flags().value<u32>();

  ImGui::Text("%-13s%s", "Runtime",
              formatDuration(absl::Seconds(state.runtime()),
                             OmitMiilliseconds | OmitMicroseconds)
                  .c_str());

  ImGui::Text("FPS %02.f", fps.getAverage());
  ImGui::SameLine(0, 50);
  ImGui::Text("Frame %lu", state.frame());
  ImGui::SameLine(0, 50);
  ImGui::Text("Tick %lu", state.tick());

  if (ImGui::CheckboxFlags("Sync frame", &flags,
                           Engine::Flags::FixedTickRateMask))
    engine->flags(static_cast<Engine::Flags::value_type>(flags));

  ImGui::PushID("EngineUiDelta");
  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text(
      "Delta %s",
      formatDuration(absl::Seconds(delta.getAverage()),
                     engine->flags().isFixedTickRate() ? OmitMicroseconds : 0)
          .c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  std::copy(delta.begin(), delta.end(), container.begin());
  ImGui::PlotLines("", container.data(), delta.size());
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text(
      "Frame %s",
      formatDuration(absl::Seconds(frameDelta.getAverage()),
                     engine->flags().isFixedTickRate() ? OmitMicroseconds : 0)
          .c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  std::copy(frameDelta.begin(), frameDelta.end(), container.begin());
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::PlotLines("", container.data(), frameDelta.size());
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text("Tick %s",
              formatDuration(absl::Seconds(tick.getAverage())).c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  std::copy(tick.begin(), tick.end(), container.begin());
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::PlotLines("", container.data(), tick.size());
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiOneQuartersWidth);
  ImGui::Text("Render %s",
              formatDuration(absl::Seconds(render.getAverage())).c_str());
  ImGui::PopItemWidth();
  ImGui::SameLine();
  std::copy(render.begin(), render.end(), container.begin());
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::PlotLines("", container.data(), render.size());
  ImGui::PopItemWidth();

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_ENGINE_UI_H_
