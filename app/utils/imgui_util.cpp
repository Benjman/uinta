#ifndef IMGUI_API_DISABLED

#include "./imgui_util.hpp"

#include <uinta/camera.hpp>
#include <uinta/gl/api.hpp>

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

void uinta::imgui::init(GLFWwindow* const window) {
#ifndef IMGUI_API_DISABLED
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
#endif  // IMGUI_API_DISABLED
}

void uinta::imgui::preRender(GLFWwindow* const window) {
#ifndef IMGUI_API_DISABLED
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
#endif  // IMGUI_API_DISABLED
}

void uinta::imgui::render(GLFWwindow* const) {
#ifndef IMGUI_API_DISABLED
#endif  // IMGUI_API_DISABLED
}

void uinta::imgui::postRender(GLFWwindow* const window) {
#ifndef IMGUI_API_DISABLED
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif  // IMGUI_API_DISABLED
}

void uinta::imgui::shutdown(GLFWwindow* const window) {
#ifndef IMGUI_API_DISABLED
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif  // IMGUI_API_DISABLED
}

void uinta::imgui::view::camera(const TargetCamera& camera) {
  auto forward = getForward(camera.pitch, camera.angle);
  auto right = getRight(camera.angle);
  auto up = getUp(forward, right);
  ImGui::Begin("Camera");
  ImGui::SetWindowSize(ImVec2(275, 300));
  ImGui::Text("Translation:   edsf");
  ImGui::Text("Angle:         wr");
  ImGui::Text("Pitch:         qa");
  ImGui::Text("Dist:          cv");
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.x.current, camera.target.y.current, camera.target.z.current);
  ImGui::Text("Dist         %+.2f", camera.dist.current);
  ImGui::Text("Pitch        %+.2f°", fmod(360 - camera.pitch, 360));
  ImGui::Text("Angle        %+.2f°", fmod(360 - camera.angle, 360));
  ImGui::Text("Forward      %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
  ImGui::Text("Right        %+.2f %+.2f %+.2f", right.x, right.y, right.z);
  ImGui::Text("Up           %+.2f %+.2f %+.2f", up.x, up.y, up.z);
  ImGui::End();
}

void uinta::imgui::view::camera(const Camera& camera) {
  ImGui::Begin("Camera");
  ImGui::SetWindowSize(ImVec2(275, 200));
  ImGui::Text("Translation:   wasd or right-mouse");
  ImGui::Text("Rotation:      cv or middle-mouse");
  ImGui::Text("Distance:      y-scroll");
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.x.current, camera.target.y.current, camera.target.z.current);
  ImGui::Text("Dist         %+.2f°", camera.dist.current);
  ImGui::Text("Pitch        %+.2f°", camera.pitch);
  ImGui::Text("Yaw          %+.2f", camera.yaw);
  ImGui::End();
}
