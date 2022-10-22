#include <uinta/gl/api.hpp>
#include <uinta/logging.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/utils/direction.hpp>

#include "./glfw_runner.hpp"
#include "./imgui.hpp"

namespace uinta {

void camera(const TargetCamera& camera);
void inputUi(const InputState& input, const Display& display);
void settings(Runner& runner);

void GlfwRunnerUi::onInit(GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::GetCurrentContext()) return;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(runner.window, true);
  ImGui_ImplOpenGL3_Init(UINTA_IMGUI_GLSL_VERSION);
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
  inputUi(runner.input, runner.display);
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

inline void inputUi(const InputState& input, const Display& display) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Input")) return;

  ImGui::Text("flags      %7i", input.flags);
  ImGui::Text("pos   (%4.0f, %4.0f)", input.cursorx, input.cursory);
  ImGui::Text("view  (%0.2f, %0.2f)", input.cursorx / (float)display.width, input.cursory / (float)display.height);
  // ImGui::Text("ndc  (%0.2f, %0.2f)", );  // TODO
  // ImGui::Text("view  (%0.2f, %0.2f)", ); // TODO
  // ImGui::Text("world (%0.2f, %0.2f)", ); // TODO

  ImGui::Separator();

  if (ImGui::TreeNode("Signals")) {
    ImGui::BeginTable("mouseUi", 2);
    ImGui::BeginDisabled(true);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_down", isFlagSet(input::HAS_KEY_DOWN, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.keys_down.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_down", isFlagSet(input::HAS_MOUSE_DOWN, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.mouse_down.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_pressed", isFlagSet(input::HAS_KEY_PRESSED, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.keys_pressed.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_pressed", isFlagSet(input::HAS_MOUSE_PRESSED, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.mouse_pressed.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_repeated", isFlagSet(input::HAS_KEY_REPEATED, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.keys_repeated.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_released", isFlagSet(input::HAS_MOUSE_RELEASED, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.mouse_released.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_released", isFlagSet(input::HAS_KEY_RELEASED, input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", input.keys_released.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_scroll", isFlagSet(input::HAS_MOUSE_SCROLL, input.flags));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_move", isFlagSet(input::HAS_MOUSE_MOVE, input.flags));

    ImGui::EndDisabled();
    ImGui::EndTable();

    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
