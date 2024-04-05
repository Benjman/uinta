#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_

#include <imgui.h>

#include <string>

#include "uinta/app_config.h"
#include "uinta/engine/engine.h"
#include "uinta/window.h"

namespace uinta {

inline void RenderPlatformUi(Platform* platform) noexcept {
  if (!ImGui::TreeNode("Platform")) return;

  std::string buildType = "Release";
#ifdef UINTA_DEBUG
  buildType = "Debug";
#endif
  ImGui::Text("%-16s%s", "Build Type", buildType.c_str());
  auto* window = platform->window();

  ImGui::SeparatorText(
      absl::StrFormat("Monitor%s", platform->monitors().size() ? "s" : "")
          .c_str());
  for (auto monitor : platform->monitors()) {
    ImGui::Text("%-10s %u x %u  @%uhz", monitor.name().c_str(),
                monitor.height(), monitor.width(), monitor.hz());
  }

  ImGui::SeparatorText(
      absl::StrFormat("Window \"%s\"", window->name()).c_str());
  ImGui::Text("%-13s %s", "Monitor", window->monitor()->name().c_str());
  ImGui::Text("%-13s %u x %u", "Position", window->x(), window->y());
  ImGui::Text("%-13s %u x %u  (%.3f aspect)", "Resolution", window->height(),
              window->width(), window->aspect());

  bool fullscreen = window->isFullscreen();
  if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
    platform->makeFullscreen(fullscreen, window);
  }

  ImGui::Separator();

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PLATFORM_UI_H_
