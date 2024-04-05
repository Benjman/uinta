#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_IMGUI_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_IMGUI_UI_H_

#include <imgui.h>

#include "uinta/debug/ui/utils.h"
#include "uinta/window.h"

namespace uinta {

inline void RenderImGuiUi(const Window* window) noexcept {
  if (!ImGui::TreeNode("ImGui")) return;

  auto& style = ImGui::GetStyle();
  auto size = ImGui::GetWindowSize();
  auto pos = ImGui::GetWindowPos();
  auto updateSizes = false;

  ImGui::PushItemWidth(UiWidth);
  updateSizes = ImGui::DragFloat("##UiWidth", &UiWidth, 1, 0, Limits.max,
                                 "UiWidth: %.0f") ||
                updateSizes;

  ImGui::Text("Position: %.0f, %.0f", pos.x, pos.y);
  ImGui::Text("Size: %.0f, %.0f", size.x, size.y);
  ImGui::Text("CellPadding: %.0f, %.0f", style.CellPadding.x,
              style.CellPadding.y);
  ImGui::Text("DisplaySafeAreaPadding: %.0f, %.0f",
              style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y);
  ImGui::Text("DisplayWindowPadding: %.0f, %.0f", style.DisplayWindowPadding.x,
              style.DisplayWindowPadding.y);
  ImGui::Text("FramePadding: %.0f, %.0f", style.FramePadding.x,
              style.FramePadding.y);
  ImGui::Text("FramePadding: %.0f, %.0f", style.FramePadding.x,
              style.FramePadding.y);
  ImGui::Text("SeparatorTextPadding: %.0f, %.0f", style.SeparatorTextPadding.x,
              style.SeparatorTextPadding.y);
  ImGui::Text("TouchExtraPadding: %.0f, %.0f", style.TouchExtraPadding.x,
              style.TouchExtraPadding.y);
  ImGui::Text("WindowPadding: %.0f, %.0f", style.WindowPadding.x,
              style.WindowPadding.y);

  static auto updatePos = true;
  if (updatePos) {
    auto size = ImGui::GetWindowSize();
    auto pos = ImGui::GetWindowPos();
    pos.x = std::min(window->width() - size.x, pos.x);
    pos.x = std::max(0.0f, pos.x);
    pos.y = std::min(window->height() - size.y, pos.y);
    pos.y = std::max(0.0f, pos.y);
    ImGui::SetWindowPos(pos);
  }
  ImGui::Checkbox("Constrain to window", &updatePos);

  ImGui::PopItemWidth();
  ImGui::TreePop();

  if (updateSizes) {
    UiHalfWidth = UiWidth / 2.0;
    UiOneThirdsWidth = UiWidth * 1.0 / 3.0;
    UiTwoThirdsWidth = UiWidth * 2.0 / 3.0;
    UiOneQuartersWidth = UiWidth * 1.0 / 4.0;
    UiThreeQuartersWidth = UiWidth * 3.0 / 4.0;
    UiOneThirdsWidth -= UiSpacing * (1.0 / 3.0);
    UiTwoThirdsWidth -= UiSpacing * (2.0 / 3.0);
    UiOneQuartersWidth -= UiSpacing * (1.0 / 4.0);
    UiThreeQuartersWidth -= UiSpacing * (3.0 / 4.0);
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_IMGUI_UI_H_
