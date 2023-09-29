#include <uinta/glfw/imgui.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/glfw/glfw_ui_scene.hpp>
#include <uinta/glfw/ui/camera.cpp>
#include <uinta/glfw/ui/input.cpp>
#include <uinta/glfw/ui/settings.cpp>
#include <uinta/glfw/ui/timing_controller.cpp>

namespace uinta {

static bool showingWindow = false;
static TimingController renderTiming;
static TimingController tickTiming;

GlfwRunnerUi::GlfwRunnerUi(GlfwRunner& runner) : Scene("GLFW UI", runner, Scene::Layer::Debug) {
}

uinta_error_code GlfwRunnerUi::init() {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::GetCurrentContext()) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GlfwRunner&>(runner()).glfwWindow(), true);
    ImGui_ImplOpenGL3_Init(UINTA_IMGUI_GLSL_VERSION);
  }
#endif  // IMGUI_API_DISABLED
  return transition(State::Running);
}

void GlfwRunnerUi::pre_tick(const RunnerState& state, const InputState& input) {
  Scene::pre_tick(state, input);
  tickTiming.start();
}

void GlfwRunnerUi::tick(const RunnerState& state, const InputState& input) {
  Scene::tick(state, input);
}

void GlfwRunnerUi::post_tick(const RunnerState& state, const InputState& input) {
  Scene::post_tick(state, input);
  tickTiming.stop();
}

void GlfwRunnerUi::pre_render(const RunnerState& state) {
  Scene::pre_render(state);
#ifndef IMGUI_API_DISABLED
  renderTiming.start();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  showingWindow = ImGui::Begin(static_cast<GlfwRunner&>(runner()).window().title.data());
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::render(const RunnerState& state) {
  Scene::render(state);
#ifndef IMGUI_API_DISABLED
  auto& io = ImGui::GetIO();
  if (showingWindow) {
    auto& runner = static_cast<GlfwRunner&>(this->runner());
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    tickTiming.render("Tick ms");
    renderTiming.render("Render ms");
    if (auto* cam = runner.find_camera(); cam)
      if (ImGui::CollapsingHeader("Camera")) ui::camera(*const_cast<TargetCamera*>(cam), state);
    if (ImGui::CollapsingHeader("Input")) inputUi(runner);
    if (ImGui::CollapsingHeader("Settings")) settings(runner);
  }
  if (io.WantCaptureKeyboard) setFlag(CaptureKeyboard, true, m_flags);
  if (io.WantCaptureMouse) setFlag(CaptureMouse, true, m_flags);
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::post_render(const RunnerState& state) {
  Scene::post_render(state);
#ifndef IMGUI_API_DISABLED
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  renderTiming.stop();
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::shutdown() {
  Scene::shutdown();
#ifndef IMGUI_API_DISABLED
  if (!ImGui::GetCurrentContext()) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
