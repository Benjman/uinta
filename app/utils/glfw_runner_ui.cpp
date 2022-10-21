#include <uinta/gl/api.hpp>
#include <uinta/logging.hpp>
#include <uinta/utils/direction.hpp>
#include <uinta/utils/running_avg.hpp>

#include "./glfw_runner.hpp"
#include "./imgui.hpp"

// TODO this has a bug where inputs handled by ImGUI continue to propagate into the engine. So sliding a camera parameter also
// moves the scene.

namespace uinta {

void camera(const Camera& camera);

// TODO Below should return an `InputEvent` to describe the input that was handled.
// Returns a `bool` representing whether it has handled an input event
bool camera(const TargetCamera& camera);

void GlfwRunnerUi::onInit(const GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(runner.window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::onPreTick(const GlfwRunner& runner) {
  tickTime_micros = runner.getRuntime();
}

void GlfwRunnerUi::onTick(const GlfwRunner& runner) {
}

void GlfwRunnerUi::onPostTick(const GlfwRunner& runner) {
  tickTime_micros = (runner.getRuntime() - tickTime_micros) * 1000000;
}

void GlfwRunnerUi::onPreRender(const GlfwRunner& runner) {
  renderTime_micros = runner.getRuntime();
}

void GlfwRunnerUi::onRender(const GlfwRunner& runner) {
}

void GlfwRunnerUi::onPostRender(const GlfwRunner& runner) {
  renderTime_micros = (runner.getRuntime() - renderTime_micros) * 1000000;
}

void GlfwRunnerUi::onShutdown(const GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

GlfwRunnerUiResult GlfwRunnerUi::updateAndRender(const GlfwRunner& runner) {
  GlfwRunnerUiResult result;
#ifndef IMGUI_API_DISABLED
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("Engine");
  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  settings(runner, result);
  if (camera(runner.camera)) setFlag(GlfwRunnerUiResult::INPUT_HANDLED, true, result.flags);
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif  // IMGUI_API_DISABLED
  return result;
}

inline bool camera(const TargetCamera& camera) {
  bool hasInput = false;
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Camera")) return false;
  auto forward = getForward(camera.pitch, camera.angle);
  auto right = getRight(camera.angle);
  auto up = getUp(forward, right);
  auto threeSixty = 360.f;

  ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false);
  ImGui::Text("Keyboard input:");
  ImGui::Text("Translation:   edsf");
  ImGui::Text("Angle:         wr");
  ImGui::Text("Pitch:         qa");
  ImGui::Text("Dist:          cv");
  ImGui::Separator();
  ImGui::Text("Mouse input:");
  ImGui::Text("Translation:    LMB");
  ImGui::Text("Angle & pitch:  MMB");
  ImGui::Text("Dist:           Scroll");
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("ChildR", ImVec2(0, 260));
  if (ImGui::DragScalar("Dist", ImGuiDataType_Float, (void*)&camera.dist.target, 0.1f, 0, &threeSixty, "%+.2f")) hasInput = true;
  if (ImGui::DragScalar("Pitch", ImGuiDataType_Float, (void*)&camera.pitch.target, 0.1f, 0, &threeSixty, "%+.2f"))
    hasInput = true;
  if (ImGui::DragScalar("Angle", ImGuiDataType_Float, (void*)&camera.angle.target, 0.1f, 0, &threeSixty, "%+.2f"))
    hasInput = true;
  ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.x.target, camera.target.y.target, camera.target.z.target);
  ImGui::Text("Forward      %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
  ImGui::Text("Right        %+.2f %+.2f %+.2f", right.x, right.y, right.z);
  ImGui::Text("Up           %+.2f %+.2f %+.2f", up.x, up.y, up.z);
  ImGui::EndChild();
#endif  // IMGUI_API_DISABLED
  return hasInput;
}

inline void GlfwRunnerUi::settings(const GlfwRunner& runner, GlfwRunnerUiResult& result) {
  if (ImGui::CollapsingHeader("Settings")) {
    if (ImGui::Checkbox("Show grid", &cb_showGrid)) setFlag(GlfwRunnerUiResult::SHOW_GRID_FLAG, true, result.flags);
  }
}

}  // namespace uinta
