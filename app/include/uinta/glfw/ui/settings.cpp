#include <uinta/glfw/imgui.h>

#include <map>
#include <uinta/glfw/glfw_runner.hpp>

namespace uinta {

void settingsGraphics(GlfwRunner &runner);
void settingsGrid(Runner &runner);
void settingsPolygonMode();

inline void settings(GlfwRunner &runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Settings")) return;
  settingsGraphics(runner);
  settingsGrid(runner);
  settingsPolygonMode();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

void settingsGraphics(GlfwRunner &runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::TreeNode("Graphics")) return;
  using AspectRatio = f32;
  struct Resolution {
    i32 width;
    i32 height;
    std::string toString() {
      return std::to_string(width) + "x" + std::to_string(height);
    }
    bool operator==(const Resolution &other) const {
      return other.width == width && other.height == height;
    }
  };
  static const std::map<AspectRatio, std::vector<Resolution>> resolutions = {
      {1.33, {{640, 480}, {800, 600}, {1024, 768}, {1152, 864}, {1280, 960}, {1400, 1050}}},
      {1.6, {{1280, 800}, {1440, 900}, {1680, 1050}, {1920, 1200}, {2560, 1600}}},
      {1.77, {{640, 360}, {960, 540}, {1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}, {2560, 1440}, {3840, 2160}}},
      {2.33, {{2560, 1080}, {3440, 1440}, {5120, 2160}}}};
  static std::vector<Resolution> arResolutions = resolutions.begin()->second;
  static Resolution selectedResolution = arResolutions.at(0);
  static i32 refreshRate = 0;
  if (refreshRate == 0) {
    refreshRate = runner.monitors.at(0).refreshRate;
    for (const auto &resolutions : resolutions) {
      if (std::abs(resolutions.first - runner.window.aspectRatio) > 0.01) continue;
      arResolutions = resolutions.second;
      Resolution windowResolution(runner.window.width, runner.window.height);
      for (auto &resolution : resolutions.second)
        if (resolution == windowResolution) selectedResolution = resolution;
      break;
    }
  }
  ImGui::Text("Refresh rate %i", refreshRate);
  if (ImGui::BeginCombo("Resolution", selectedResolution.toString().c_str())) {
    for (size_t n = 0; n < arResolutions.size(); ++n) {
      const auto isSelected = selectedResolution == arResolutions.at(n);
      if (ImGui::Selectable(arResolutions.at(n).toString().c_str(), isSelected)) {
        selectedResolution = arResolutions.at(n);
      }
    }
    ImGui::EndCombo();
  }
  ImGui::CheckboxFlags("Fullscreen", &runner.window.flags, Window::FULLSCREEN);
  if (ImGui::Button("Apply")) {
    runner.window = Window(runner.window.title, selectedResolution.width, selectedResolution.height);
    runner.monitors.at(0).refreshRate = refreshRate;
    GLFWmonitor *mon = nullptr;
    if (isFlagSet(Window::FULLSCREEN, runner.window.flags)) mon = runner.monitors.at(0).ptr;
    glfwSetWindowMonitor(runner.glfwWindow, mon, 0, 0, runner.window.width, runner.window.height,
                         runner.monitors.at(0).refreshRate);
    glViewport(0, 0, selectedResolution.width, selectedResolution.height);
  }
  ImGui::TreePop();
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
