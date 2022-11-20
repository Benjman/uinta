#include <uinta/glfw_utils/imgui.h>

#include <glm/glm.hpp>
#include <uinta/camera/utils.hpp>
#include <uinta/gl/api.hpp>
#include <uinta/glfw_utils/glfw_runner.hpp>
#include <uinta/glfw_utils/glfw_runner_ui.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

const struct {
  float min = -INFINITY;
  float zero = 0;
  float one_tenth = 0.1;
  float twenty = 20;
  float max = INFINITY;
} limits;

inline void camera(TargetCamera& camera);
inline void cameraClippingPlanes(TargetCamera& camera);
inline void cameraHotkeys(TargetCamera& camera);
inline void cameraTransform(TargetCamera& camera);

void inputUi(Runner& runner);
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
#ifndef IMGUI_API_DISABLED
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  showingWindow = ImGui::Begin(runner.display.title.data());
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::onRender(GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  auto& io = ImGui::GetIO();
  if (showingWindow) {
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    camera(runner.camera);
    inputUi(runner);
    settings(runner);
  }
  if (io.WantCaptureKeyboard) setFlag(INPUT_HANDLED_KEYBOARD, true, flags);
  if (io.WantCaptureMouse) setFlag(INPUT_HANDLED_MOUSE, true, flags);
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::onPostRender(GlfwRunner& runner) {
  renderTime_micros = (runner.getRuntime() - renderTime_micros) * 1000000;
#ifndef IMGUI_API_DISABLED
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif  // IMGUI_API_DISABLED
}

void GlfwRunnerUi::onShutdown(GlfwRunner& runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::GetCurrentContext()) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

inline void camera(TargetCamera& camera) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::CollapsingHeader("Camera")) {
    ImGui::PushItemWidth(200);
    cameraClippingPlanes(camera);
    cameraHotkeys(camera);
    cameraTransform(camera);
    ImGui::PopItemWidth();
  }
#endif  // IMGUI_API_DISABLED
}

inline void settings(Runner& runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::CollapsingHeader("Settings")) {
    ImGui::CheckboxFlags("Show grid", &runner.flags, RUNNER_FLAG_GRID);
  }
#endif  // IMGUI_API_DISABLED
}

inline void inputUi(Runner& runner) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Input")) return;

  if (ImGui::TreeNode("Cursor info")) {
    auto view = getViewMatrix(runner.camera);
    auto proj = getPerspectiveMatrix(runner.camera, runner.display);

    glm::vec2 cursor = {runner.input.cursorx, runner.input.cursory};
    glm::vec2 viewport = {runner.display.width, runner.display.height};
    glm::vec3 ndc = {(2 * cursor.x) / viewport.x - 1, 1 - (2 * cursor.y) / viewport.y, 1};
    auto worldRay = getWorldRay(cursor, viewport, view, proj);
    auto worldPoint = getPlaneInterceptPoint(glm::vec3(0), WORLD_UP, runner.camera.position, worldRay);

    ImGui::Text("Screen          (%5.0f, %5.0f)", runner.input.cursorx, runner.input.cursory);
    ImGui::Text("Device coord    (%5.2f, %5.2f, %5.2f)", ndc.x, ndc.y, ndc.z);
    ImGui::Text("World ray       (%5.2f, %5.2f, %5.2f)", worldRay.x, worldRay.y, worldRay.z);
    ImGui::Text("y=0 intersect   (%5.2f, %5.2f, %5.2f)", worldPoint.x, worldPoint.y, worldPoint.z);

    ImGui::TreePop();
    ImGui::Separator();
  }

  if (ImGui::TreeNode("Signals")) {
    ImGui::PushItemWidth(400);
    ImGui::BeginTable("mouseUi", 2);
    ImGui::BeginDisabled(true);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_down", isFlagSet(input::HAS_KEY_DOWN, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.keys_down.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_down", isFlagSet(input::HAS_MOUSE_DOWN, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.mouse_down.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_pressed", isFlagSet(input::HAS_KEY_PRESSED, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.keys_pressed.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_pressed", isFlagSet(input::HAS_MOUSE_PRESSED, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.mouse_pressed.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_repeated", isFlagSet(input::HAS_KEY_REPEATED, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.keys_repeated.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_released", isFlagSet(input::HAS_MOUSE_RELEASED, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.mouse_released.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_released", isFlagSet(input::HAS_KEY_RELEASED, runner.input.flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input.keys_released.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_scroll", isFlagSet(input::HAS_MOUSE_SCROLL, runner.input.flags));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Checkbox("GLFW::WantCaptureKeyboard", &ImGui::GetIO().WantCaptureKeyboard);
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_move", isFlagSet(input::HAS_MOUSE_MOVE, runner.input.flags));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Checkbox("GLFW::WantCaptureMouse", &ImGui::GetIO().WantCaptureMouse);

    ImGui::EndDisabled();
    ImGui::EndTable();

    ImGui::PopItemWidth();
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

#ifndef IMGUI_API_DISABLED
inline void cameraClippingPlanes(TargetCamera& camera) {
  if (ImGui::TreeNode("Clipping planes")) {
    float cameraClippingValues[] = {camera.config.nearPlane, camera.config.farPlane};
    ImGui::Text("Defines the boundaries of the near and far rendering planes.");
    ImGui::DragFloat2("Clipping planes", cameraClippingValues, 0.05, camera.config.farPlane, camera.config.nearPlane, "%+.1f",
                      ImGuiSliderFlags_AlwaysClamp);
    camera.config.nearPlane = cameraClippingValues[0];
    camera.config.farPlane = cameraClippingValues[1];
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

inline void cameraHotkeys(TargetCamera& camera) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Hotkeys")) {
    ImGui::Text("Translation:   edsf or LMB");
    ImGui::Text("Angle:         wr or RMB");
    ImGui::Text("Pitch:         qa or RMB");
    ImGui::Text("Dist:          cv or Scroll");

    ImGui::Separator();
    ImGui::Text("TODO Customizable hotkeys");  // TODO

    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

inline void cameraTransform(TargetCamera& camera) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Transform")) {
    if (ImGui::DragScalar("Translation agility", ImGuiDataType_Float, reinterpret_cast<void*>(&camera.angle.agility), 0.1f,
                          &limits.min, &limits.max, "%+.2f")) {
      camera.dist.agility = camera.angle.agility;
      camera.pitch.agility = camera.angle.agility;
      camera.target.x.agility = camera.angle.agility;
      camera.target.y.agility = camera.angle.agility;
      camera.target.z.agility = camera.angle.agility;
    }

    ImGui::DragScalar("Dist", ImGuiDataType_Float, reinterpret_cast<void*>(&camera.dist.target), 0.1f, &limits.one_tenth,
                      &limits.twenty, "%+.2f");
    ImGui::SameLine();
    ImGui::CheckboxFlags("Limit dist", (uint*)&camera.config.flags, CAMERA_DIST_LIMIT);

    ImGui::DragScalar("Pitch", ImGuiDataType_Float, reinterpret_cast<void*>(&camera.pitch.target), 0.1f, &limits.min, &limits.max,
                      "%+.2f");
    ImGui::SameLine();
    ImGui::CheckboxFlags("Limit pitch", (uint*)&camera.config.flags, CAMERA_PITCH_LIMIT);

    ImGui::DragScalar("Angle", ImGuiDataType_Float, reinterpret_cast<void*>(&camera.angle.target), 0.1f, &limits.min, &limits.max,
                      "%+.2f");

    ImGui::DragScalar("Speed factor", ImGuiDataType_Float, reinterpret_cast<void*>(&camera.config.translationSpeedDistFactor),
                      0.005f, &limits.zero, &limits.max, "%+.2f");
    ImGui::DragScalar("Speed factor min", ImGuiDataType_Float,
                      reinterpret_cast<void*>(&camera.config.translationSpeedDistFactorMin), 0.005f, &limits.zero, &limits.max,
                      "%+.2f");
    ImGui::Text("Speed scalar  %+.2f", calculateTranslationFactor(camera));

    auto forward = getForward(camera.pitch, camera.angle);
    auto right = getRight(camera.angle);
    auto up = getUp(forward, right);
    ImGui::Text("Position      %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
    ImGui::Text("Target        %+.2f %+.2f %+.2f", camera.target.x.target, camera.target.y.target, camera.target.z.target);
    ImGui::Text("Forward       %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
    ImGui::Text("Right         %+.2f %+.2f %+.2f", right.x, right.y, right.z);
    ImGui::Text("Up            %+.2f %+.2f %+.2f", up.x, up.y, up.z);

    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
