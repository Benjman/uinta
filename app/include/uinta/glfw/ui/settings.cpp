#include <uinta/flags.h>
#include <uinta/glfw/imgui.h>

#include <uinta/runner/runner.hpp>

namespace uinta {

void settings(Runner &runner);
void settingsGrid(Runner &runner);
void settingsPolygonMode();

inline void settings(Runner &runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Settings")) return;
  settingsGrid(runner);
  settingsPolygonMode();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

void settingsGrid(Runner &runner) {
#ifndef IMGUI_API_DISABLED
  ImGui::CheckboxFlags("Grid", &runner.flags, Runner::GRID_ENABLED);
  if (!isFlagSet(Runner::GRID_ENABLED, runner.flags)) return;
  ImGui::SameLine();
  ImGui::PushItemWidth(100);
  ImGui::DragFloat("Thickness", &runner.grid.lineWidth, 0.5, 0.5, 15.0, "%0.1f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::PopItemWidth();
#endif  // IMGUI_API_DISABLED
}

void settingsPolygonMode() {
#ifndef IMGUI_API_DISABLED
  static const GLenum polygonModesEnums[] = {GL_LINE, GL_FILL, GL_POINT};
  static const char *polygonModesStr[] = {"GL_LINE", "GL_FILL", "GL_POINT"};
  static auto polygonModeSelected = 1u;
  static auto polygonModeSize = 1.0f;
  ImGui::PushItemWidth(80);
  if (ImGui::BeginCombo("Polygon mode", polygonModesStr[polygonModeSelected])) {
    for (u32 n = 0; n < IM_ARRAYSIZE(polygonModesStr); ++n) {
      auto isSelected = polygonModeSelected == n;
      if (ImGui::Selectable(polygonModesStr[n], isSelected)) {
        polygonModeSelected = n;
        glPolygonMode(GL_FRONT_AND_BACK, polygonModesEnums[n]);
      }
      if (isSelected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  if (polygonModeSelected != 1) {
    ImGui::SameLine();
    auto label = std::string(polygonModesStr[polygonModeSelected]) + " size";
    if (ImGui::DragFloat(label.c_str(), &polygonModeSize, 0.15, 0.5, 15.0, "%0.1f", ImGuiSliderFlags_AlwaysClamp)) {
      if (polygonModeSelected == 0)
        glLineWidth(polygonModeSize);
      else
        glPointSize(polygonModeSize);
    }
    ImGui::PopItemWidth();
  }
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
