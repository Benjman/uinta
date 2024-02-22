#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_

#include <imgui.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/scene.h"

namespace uinta {

struct SceneUiInfo final {
  std::unordered_map<Scene*, Scene::Layer> SelectedLayer;
  std::unordered_map<Scene*, Scene::State> SelectedState;
} static info;

inline std::string sptraddress(void* ptr) noexcept {
  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%p", ptr);
  return buffer;
}

inline void RenderChildrenList(Scene* scene, const auto&) noexcept;
inline void RenderComponentInfo(Scene* scene) noexcept;
inline void RenderLayer(Scene* scene) noexcept;
inline void RenderState(Scene* scene) noexcept;
inline void RenderSystemInfo(Scene* scene) noexcept;

inline void RenderScene(const auto& state, Scene* scene) noexcept {
  if (!scene || !ImGui::TreeNode(scene->name().c_str())) return;
  scene->onDebugUi(state);
  RenderComponentInfo(scene);
  RenderSystemInfo(scene);
  RenderLayer(scene);
  RenderState(scene);
  RenderChildrenList(state, scene);
  ImGui::TreePop();
}

inline void RenderChildrenList(const auto& state, Scene* scene) noexcept {
  auto& children = scene->children().stack();
  if (children.size()) {
    if (ImGui::TreeNode(StrFormat("Children: %zu", children.size()).c_str())) {
      std::for_each(children.begin(), children.end(),
                    [&state](auto& scene) { RenderScene(state, scene.get()); });
      ImGui::TreePop();
    }
  } else {
    ImGui::Text("Children: 0");
  }
}

inline void RenderComponentInfo(Scene* scene) noexcept {
  ImGui::Text("Components: %zu", scene->components()->allSize());
}

inline void RenderSystemInfo(Scene* scene) noexcept {
  ImGui::Text("Systems: %zu", scene->systems()->size());
}

inline void RenderSceneUi(auto* engine, const auto& state) noexcept {
  if (!engine->scenes()->empty())
    RenderScene(state, engine->scenes()->front().get());
}

inline void RenderLayer(Scene* scene) noexcept {
  auto editId = StrFormat("%s_editlayer", scene->name());
  ImGui::PushID(editId.c_str());
  if (!info.SelectedLayer.contains(scene))
    info.SelectedLayer.emplace(scene, scene->layer());
  ImGui::PushID((editId + "Layer").c_str());
  if (ImGui::BeginCombo("", to_string(info.SelectedLayer.at(scene)).c_str())) {
    for (size_t i = 0; i < Scene::Layers.size(); i++) {
      auto isSelected = static_cast<size_t>(info.SelectedLayer.at(scene)) == i;
      if (ImGui::Selectable(to_string(Scene::Layers[i]).c_str(), isSelected))
        info.SelectedLayer.at(scene) = static_cast<Scene::Layer>(i);
      if (isSelected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
  ImGui::SameLine();
  if (ImGui::Button("Update")) {
    if (auto status = scene->layer(info.SelectedLayer.at(scene));
        !status.ok()) {
      LOG(WARNING) << status.message();
    }
    ImGui::CloseCurrentPopup();
  }
  ImGui::PopID();
}

inline void RenderState(Scene* scene) noexcept {
  auto editId = StrFormat("%s_editstate", scene->name());
  ImGui::PushID(editId.c_str());
  if (!info.SelectedState.contains(scene))
    info.SelectedState.emplace(scene, scene->state());
  ImGui::PushID((editId + "State").c_str());
  if (ImGui::BeginCombo("", to_string(info.SelectedState.at(scene)).c_str())) {
    for (size_t i = 0; i < Scene::States.size(); i++) {
      auto isSelected = static_cast<size_t>(info.SelectedState.at(scene)) == i;
      if (ImGui::Selectable(to_string(Scene::States[i]).c_str(), isSelected))
        info.SelectedState.at(scene) = static_cast<Scene::State>(i);
      if (isSelected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
  ImGui::SameLine();
  if (ImGui::Button("Update")) {
    if (auto status = scene->state(info.SelectedState.at(scene));
        !status.ok()) {
      LOG(WARNING) << status.message();
    }
    ImGui::CloseCurrentPopup();
  }
  ImGui::PopID();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_SCENE_UI_H_
