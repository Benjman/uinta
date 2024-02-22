#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_

#include <imgui.h>

#include <string>

#include "uinta/engine/engine.h"

namespace uinta {

inline void RenderPlatformUi(const Platform* platform) noexcept {
  if (!ImGui::TreeNode("Platform")) return;

  ImGui::SeparatorText("Window");
  auto* window = platform->window();
  ImGui::Text("%s\"%s\"", "", window->name().c_str());
  ImGui::Text("%-15s %u x %u  (%.3f aspect)", "Resolution", window->height(),
              window->width(), window->aspect());
  ImGui::Text("%-15s%15p", "GLFW Ref", window->userData());
  ImGui::Text("%-16s%s", "Status", window->status().ToString().c_str());
  ImGui::Separator();

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_
