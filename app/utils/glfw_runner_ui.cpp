#include <uinta/gl/api.hpp>
#include <uinta/logging.hpp>
#include <uinta/math/utils.hpp>

#include "./glfw_runner.hpp"

#ifndef IMGUI_API_DISABLED

#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// FIXME there has to be a better way to point to library source files
#include "../../lib/imgui/backends/imgui_impl_glfw.cpp"
#include "../../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../../lib/imgui/imgui.cpp"
#include "../../lib/imgui/imgui_draw.cpp"
#include "../../lib/imgui/imgui_tables.cpp"
#include "../../lib/imgui/imgui_widgets.cpp"

#endif  // IMGUI_API_DISABLED

namespace uinta {

void camera(const TargetCamera& camera);
void settings(Runner& runner);

void GlfwRunnerUi::onInit(GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::GetCurrentContext()) return;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(runner.window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::onPreTick(GlfwRunner& runner) {
  tickTime_micros = runner.getRuntime();
}

void GlfwRunnerUi::onTick(GlfwRunner& runner) {
}

void GlfwRunnerUi::onPostTick(GlfwRunner& runner) {
  tickTime_micros = (runner.getRuntime() - tickTime_micros) * 1000000;
}

void GlfwRunnerUi::onPreRender(GlfwRunner& runner) {
  renderTime_micros = runner.getRuntime();
}

void GlfwRunnerUi::onRender(GlfwRunner& runner) {
}

void GlfwRunnerUi::onPostRender(GlfwRunner& runner) {
  renderTime_micros = (runner.getRuntime() - renderTime_micros) * 1000000;
}

void GlfwRunnerUi::onShutdown(GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::GetCurrentContext()) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

flags_t GlfwRunnerUi::updateAndRender(GlfwRunner& runner) {
  flags_t flags = 0;
#ifndef IMGUI_API_DISABLED
  auto& io = ImGui::GetIO();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin(std::string(runner.display.title).c_str());
  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  camera(runner.camera);
  settings(runner);
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  if (io.WantCaptureKeyboard || io.WantCaptureMouse) setFlag(GLFW_RUNNER_UI_INPUT_HANDLED, true, flags);
#endif  // IMGUI_API_DISABLED
  return flags;
}

inline void camera(const TargetCamera& camera) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Camera")) return;
  auto forward = getForward(camera.pitch, camera.angle);
  auto right = getRight(camera.angle);
  auto up = getUp(forward, right);
  auto threeSixty = 360.f;

  ImGui::BeginChild("ChildL", {ImGui::GetContentRegionAvail().x * 0.5f, 260}, false);
  ImGui::Text("Keyboard");
  ImGui::Text("Translation:   edsf");
  ImGui::Text("Angle:         wr");
  ImGui::Text("Pitch:         qa");
  ImGui::Text("Dist:          cv");
  ImGui::Separator();
  ImGui::Text("Mouse");
  ImGui::Text("Translation:    LMB");
  ImGui::Text("Angle & pitch:  RMB");
  ImGui::Text("Dist:           Scroll");
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("ChildR", {0, 260});
  ImGui::DragScalar("Dist", ImGuiDataType_Float, (void*)&camera.dist.target, 0.1f, 0, &threeSixty, "%+.2f");
  ImGui::DragScalar("Pitch", ImGuiDataType_Float, (void*)&camera.pitch.target, 0.1f, 0, &threeSixty, "%+.2f");
  ImGui::DragScalar("Angle", ImGuiDataType_Float, (void*)&camera.angle.target, 0.1f, 0, &threeSixty, "%+.2f");
  ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.x.target, camera.target.y.target, camera.target.z.target);
  ImGui::Text("Forward      %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
  ImGui::Text("Right        %+.2f %+.2f %+.2f", right.x, right.y, right.z);
  ImGui::Text("Up           %+.2f %+.2f %+.2f", up.x, up.y, up.z);
  ImGui::EndChild();
#endif  // IMGUI_API_DISABLED
}

inline void settings(Runner& runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::CollapsingHeader("Settings")) {
    ImGui::CheckboxFlags("Show grid", &runner.flags, RUNNER_FLAG_GRID);
  }
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
