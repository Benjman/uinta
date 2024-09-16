#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_UI_H_

#include <imgui.h>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/debug/ui/utils.h"
#include "uinta/input/input.h"
#include "uinta/window.h"

namespace uinta {

inline void RenderInputUi(const Input* input, const Window* window) noexcept {
  if (!ImGui::TreeNode("Input")) return;

  glm::vec2 cursor(input->cursorx(), input->cursory());
  glm::vec2 viewport(window->width(), window->height());
  glm::vec3 ndc((2 * cursor.x) / viewport.x - 1,
                1 - (2 * cursor.y) / viewport.y, 1);
  auto world = input->cursorWorldPoint();

  ImGui::Text("%-7s(%5.0f, %5.0f)", "Screen", input->cursorx(),
              input->cursory());
  ImGui::Text("%-7s(%5.2f, %5.2f, %5.2f)", "NDC", ndc.x, ndc.y, ndc.z);
  ImGui::Text("%-7s(%5.2f, %5.2f, %5.2f)", "y=0", world.x, world.y, world.z);

  ImGui::RadioButton("ImGUI Keyboard", ImGui::GetIO().WantCaptureKeyboard);
  ImGui::SameLine(UiTwoThirdsWidth);
  ImGui::RadioButton("ImGUI Mouse", ImGui::GetIO().WantCaptureMouse);
  ImGui::RadioButton("Key down", input->flags().isKeyDown());
  ImGui::SameLine(UiTwoThirdsWidth);
  ImGui::RadioButton("Key pressed", input->flags().isKeyPressed());
  ImGui::RadioButton("Key released", input->flags().isKeyReleased());
  ImGui::SameLine(UiTwoThirdsWidth);
  ImGui::RadioButton("Key repeated", input->flags().isKeyRepeated());
  ImGui::RadioButton("Mouse down", input->flags().isMouseDown());
  ImGui::SameLine(UiTwoThirdsWidth);
  ImGui::RadioButton("Mouse move", input->flags().isMouseMove());
  ImGui::RadioButton("Mouse pressed", input->flags().isMousePressed());
  ImGui::SameLine(UiTwoThirdsWidth);
  ImGui::RadioButton("Mouse released", input->flags().isMouseReleased());
  ImGui::RadioButton("Mouse scroll", input->flags().isMouseScroll());

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_INPUT_UI_H_
