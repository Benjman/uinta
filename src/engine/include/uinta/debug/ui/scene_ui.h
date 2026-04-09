#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_

#include <absl/strings/str_format.h>
#include <imgui.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

#include "uinta/scene/scene.h"

namespace uinta {

struct SceneUiInfo final {
  std::unordered_map<Scene*, SceneLayer> SelectedLayer;
  std::unordered_map<Scene*, SceneState> SelectedState;
} static info;

inline std::string sptraddress(void* ptr) noexcept {
  std::array<char, 50> buffer;
  snprintf(buffer.data(), sizeof(buffer), "%p", ptr);
  return buffer.data();
}

inline void RenderChildrenList(Scene* scene) noexcept;
inline void RenderComponentInfo(Scene* scene) noexcept;
inline void RenderLayer(Scene* scene) noexcept;
inline void RenderState(Scene* scene) noexcept;

inline void RenderScene(Scene* scene) noexcept {
  if ((scene == nullptr) || !ImGui::TreeNode(scene->name().c_str())) {
    return;
  }
  scene->onDebugUi();
  RenderLayer(scene);
  RenderState(scene);
  RenderComponentInfo(scene);
  RenderChildrenList(scene);
  ImGui::TreePop();
}

inline void RenderChildrenList(Scene* scene) noexcept {
  auto& children = scene->children().stack();
  if (children.size() != 0u) {
    if (ImGui::TreeNode(
            absl::StrFormat("Children: %zu", children.size()).c_str())) {
      std::ranges::for_each(children.begin(), children.end(),
                            [](auto& scene) { RenderScene(scene.get()); });
      ImGui::TreePop();
    }
  }
}

inline void RenderComponentInfo(Scene* scene) noexcept {
  if (auto count = scene->components()->allSize()) {
    ImGui::Text("Components: %zu", count);
  }
}

inline void RenderSceneUi(auto* engine) noexcept {
  if (!engine->scenes()->empty()) {
    RenderScene(engine->scenes()->front().get());
  }
}

inline void RenderLayer(Scene* scene) noexcept {
  auto editId = absl::StrFormat("%s_editlayer", scene->name());
  ImGui::PushID(editId.c_str());
  if (!info.SelectedLayer.contains(scene)) {
    info.SelectedLayer.emplace(scene, scene->layer());
  }
  ImGui::PushID((editId + "Layer").c_str());
  if (ImGui::BeginCombo("", to_string(info.SelectedLayer.at(scene)).c_str())) {
    for (size_t i = 0; i < SceneLayers.size(); i++) {
      auto isSelected = static_cast<size_t>(info.SelectedLayer.at(scene)) == i;
      if (ImGui::Selectable(to_string(SceneLayers[i]).c_str(), isSelected)) {
        info.SelectedLayer.at(scene) = static_cast<SceneLayer>(i);
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
  ImGui::SameLine();
  if (ImGui::Button("Update")) {
    scene->layer(info.SelectedLayer.at(scene));
    ImGui::CloseCurrentPopup();
  }
  ImGui::PopID();
}

inline void RenderState(Scene* scene) noexcept {
  auto editId = absl::StrFormat("%s_editstate", scene->name());
  ImGui::PushID(editId.c_str());
  if (!info.SelectedState.contains(scene)) {
    info.SelectedState.emplace(scene, scene->state());
  }
  ImGui::PushID((editId + "State").c_str());
  if (ImGui::BeginCombo("", to_string(info.SelectedState.at(scene)).c_str())) {
    for (size_t i = 0; i < SceneStates.size(); i++) {
      auto isSelected = static_cast<size_t>(info.SelectedState.at(scene)) == i;
      if (ImGui::Selectable(to_string(SceneStates[i]).c_str(), isSelected)) {
        info.SelectedState.at(scene) = static_cast<SceneState>(i);
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
  ImGui::SameLine();
  if (ImGui::Button("Update")) {
    scene->state(info.SelectedState.at(scene));
    ImGui::CloseCurrentPopup();
  }
  ImGui::PopID();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_
