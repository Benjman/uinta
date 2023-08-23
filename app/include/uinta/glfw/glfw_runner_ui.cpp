#include <uinta/glfw/imgui.h>

#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/glfw/glfw_runner_ui.hpp>
#include <uinta/glfw/ui/camera.cpp>
#include <uinta/glfw/ui/input.cpp>
#include <uinta/glfw/ui/scene.cpp>
#include <uinta/glfw/ui/settings.cpp>
#include <uinta/glfw/ui/timing_controller.cpp>

namespace uinta {

static bool showingWindow = false;
static TimingController renderTiming;
static TimingController tickTiming;

void ui::onInit(GlfwRunner &runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::GetCurrentContext()) return;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(runner.window, true);
  ImGui_ImplOpenGL3_Init(UINTA_IMGUI_GLSL_VERSION);
#endif  // IMGUI_API_DISABLED
}

void ui::onPreTick(GlfwRunner &runner, const RunnerState &state) {
  tickTiming.start();
}

void ui::onTick(GlfwRunner &runner, const RunnerState &state) {
}

void ui::onPostTick(GlfwRunner &runner, const RunnerState &state) {
  tickTiming.stop();
}

void ui::onPreRender(GlfwRunner &runner, const RunnerState &state) {
#ifndef IMGUI_API_DISABLED
  renderTiming.start();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  showingWindow = ImGui::Begin(runner.display.title.data());
#endif  // IMGUI_API_DISABLED
}

void ui::onRender(GlfwRunner &runner, const RunnerState &state) {
#ifndef IMGUI_API_DISABLED
  auto &io = ImGui::GetIO();
  if (showingWindow) {
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    tickTiming.render("Tick ms");
    renderTiming.render("Render ms");
    camera(runner.scene.camera, state);
    inputUi(runner);
    scene(runner);
    settings(runner);
  }
  if (io.WantCaptureKeyboard) setFlag(INPUT_HANDLED_KEYBOARD, true, flags);
  if (io.WantCaptureMouse) setFlag(INPUT_HANDLED_MOUSE, true, flags);
#endif  // IMGUI_API_DISABLED
}

void ui::onPostRender(GlfwRunner &runner, const RunnerState &state) {
#ifndef IMGUI_API_DISABLED
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  renderTiming.stop();
#endif  // IMGUI_API_DISABLED
}

void ui::onShutdown(GlfwRunner &runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::GetCurrentContext()) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
