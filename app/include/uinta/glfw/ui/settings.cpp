#include <uinta/glfw/imgui.h>

#include <map>
#include <uinta/exception.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/grid.hpp>

namespace uinta {

void settingsGraphics(GlfwRunner &runner);
void settingsGrid(GridScene *grid);  // FIXME: Scene stack victim
void settingsPolygonMode();

inline void settings(GlfwRunner &runner) {
#ifndef IMGUI_API_DISABLED
  settingsGraphics(runner);
  if (auto *grid = runner.find_scene<GridScene>(); grid) settingsGrid(grid);
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
    refreshRate = runner.monitors().at(0).refreshRate;
    for (const auto &resolutions : resolutions) {
      if (std::abs(resolutions.first - runner.window().aspect_ratio) > 0.01) continue;
      arResolutions = resolutions.second;
      Resolution windowResolution(runner.window().width, runner.window().height);
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
  static auto fullscreen = runner.window().fullscreen;
  ImGui::Checkbox("Fullscreen", &fullscreen);
  if (ImGui::Button("Apply")) {
    const auto window = Window(runner.window().title, selectedResolution.width, selectedResolution.height, fullscreen);
    runner.window(Window(window));
    auto monitors = runner.monitors();
    monitors.at(0).refreshRate = refreshRate;
    runner.monitors(monitors);
    GLFWmonitor *mon = nullptr;
    if (runner.window().fullscreen) mon = runner.monitors().at(0).ptr;
    const auto *const view = glfwGetVideoMode(runner.monitors().at(0).ptr);
    int x = view ? view->width / 2.0 + window.width / 2.0 : 0;
    int y = view ? view->height / 2.0 + window.height / 2.0 : 0;
    glfwSetWindowMonitor(runner.glfwWindow(), mon, x, y, runner.window().width, runner.window().height,
                         runner.monitors().at(0).refreshRate);
    glViewport(0, 0, selectedResolution.width, selectedResolution.height);
  }
  ImGui::TreePop();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

void settingsGrid(GridScene *grid) {
#ifndef IMGUI_API_DISABLED
  auto enabled = grid->state() == Scene::State::Running;
  if (ImGui::Checkbox("Grid", &enabled))
    if (auto error = grid->transition(enabled ? Scene::State::Running : Scene::State::Paused); error) throw UintaException(error);
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
