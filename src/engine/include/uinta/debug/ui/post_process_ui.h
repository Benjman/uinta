#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_POST_PROCESS_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_POST_PROCESS_UI_H_

#include <imgui.h>

#include <string>

#include "uinta/post_process/post_processor.h"

namespace uinta {

inline void RenderPostProcessUi(PostProcessor* processor) noexcept {
  if (processor == nullptr) {
    return;
  }
  if (!ImGui::TreeNode("Post Processing")) {
    return;
  }

  ImGui::Text("Target: %u x %u (HDR RGBA16F)", processor->width(),
              processor->height());
  ImGui::Text("Near: %.3f  Far: %.1f", processor->near(), processor->far());
  ImGui::Separator();

  auto& passes = processor->passes();
  if (passes.empty()) {
    ImGui::TextWrapped(
        "No passes registered. The simulation color target is being blitted "
        "directly to the default framebuffer.");
    ImGui::TreePop();
    return;
  }

  for (size_t i = 0; i < passes.size(); ++i) {
    auto& pass = passes[i];
    ImGui::PushID(static_cast<int>(i));

    bool enabled = pass->enabled();
    std::string label{pass->name()};
    if (ImGui::Checkbox(label.c_str(), &enabled)) {
      pass->enabled(enabled);
    }

    if (ImGui::TreeNode("details")) {
      pass->onDebugUi();
      ImGui::TreePop();
    }

    ImGui::PopID();
  }

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_POST_PROCESS_UI_H_
