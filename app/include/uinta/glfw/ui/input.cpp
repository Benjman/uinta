#include <uinta/glfw/imgui.h>
#include <uinta/glfw/ui/limits.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/target_camera.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline void inputUi(Runner& runner) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Cursor info")) {
    if (const auto* ccamera = runner.find_camerac(); ccamera) {
      const auto& camera = *ccamera;
      auto view = camera.view_matrix();
      auto proj = camera.perspective_matrix();

      glm::vec2 cursor = {runner.input().cursorx, runner.input().cursory};
      glm::vec2 viewport = {runner.window().width, runner.window().height};
      glm::vec3 ndc = {(2 * cursor.x) / viewport.x - 1, 1 - (2 * cursor.y) / viewport.y, 1};
      auto worldRay = getWorldRay(cursor, viewport, view, proj);
      auto worldPoint = getPlaneInterceptPoint(glm::vec3(0), WORLD_UP, camera.position(), worldRay);

      ImGui::Text("Screen          (%5.0f, %5.0f)", runner.input().cursorx, runner.input().cursory);
      ImGui::Text("Device coord    (%5.2f, %5.2f, %5.2f)", ndc.x, ndc.y, ndc.z);
      ImGui::Text("World ray       (%5.2f, %5.2f, %5.2f)", worldRay.x, worldRay.y, worldRay.z);
      ImGui::Text("y=0 intersect   (%5.2f, %5.2f, %5.2f)", worldPoint.x, worldPoint.y, worldPoint.z);

      ImGui::TreePop();
      ImGui::Separator();
    }
  }

  if (ImGui::TreeNode("Signals")) {
    ImGui::PushItemWidth(400);
    ImGui::BeginTable("mouseUi", 2);
    ImGui::BeginDisabled(true);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_down", isFlagSet(input::HAS_KEY_DOWN, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().keys_down.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_down", isFlagSet(input::HAS_MOUSE_DOWN, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().mouse_down.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_pressed", isFlagSet(input::HAS_KEY_PRESSED, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().keys_pressed.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_pressed", isFlagSet(input::HAS_MOUSE_PRESSED, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().mouse_pressed.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_repeated", isFlagSet(input::HAS_KEY_REPEATED, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().keys_repeated.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_released", isFlagSet(input::HAS_MOUSE_RELEASED, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().mouse_released.size());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::RadioButton("key_released", isFlagSet(input::HAS_KEY_RELEASED, runner.input().flags));
    ImGui::SameLine();
    ImGui::Text("(%lu)", runner.input().keys_released.size());
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_scroll", isFlagSet(input::HAS_MOUSE_SCROLL, runner.input().flags));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Checkbox("ImGUI::WantCaptureKeyboard", &ImGui::GetIO().WantCaptureKeyboard);
    ImGui::TableSetColumnIndex(1);
    ImGui::RadioButton("mouse_move", isFlagSet(input::HAS_MOUSE_MOVE, runner.input().flags));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Checkbox("ImGUI::WantCaptureMouse", &ImGui::GetIO().WantCaptureMouse);

    ImGui::EndDisabled();
    ImGui::EndTable();

    ImGui::PopItemWidth();
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
