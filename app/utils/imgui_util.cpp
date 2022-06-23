#include "imgui_utils.hpp"

#include <uinta/camera.hpp>

#include <imgui.h>

void uinta::imguiCamera(const Camera &camera) {
  ImGui::Begin("Camera");
  ImGui::SetWindowSize(ImVec2(275, 200));
  ImGui::Text("Translation:   wasd or right-mouse");
  ImGui::Text("Rotation:      cv or middle-mouse");
  ImGui::Text("Distance:      y-scroll");
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.values[0].current, camera.target.values[1].current,
              camera.target.values[2].current);
  ImGui::Text("Pitch        %+.2f", camera.pitch);
  ImGui::Text("Yaw          %+.2f", camera.yaw);
  ImGui::End();
}
