#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PRIMITIVE_SHADER_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PRIMITIVE_SHADER_UI_H_

#include <imgui.h>

#include "uinta/debug/debug_scene.h"
#include "uinta/debug/ui/utils.h"
#include "uinta/gl.h"
#include "uinta/shaders/primitive.h"

namespace uinta {

struct Flags final {
  using value_type = u32;

  static constexpr value_type Normals = 1 << 0;
  static constexpr value_type Polygons = 1 << 1;

  bool isNormals() const noexcept { return flags_ & Normals; }
  void isNormals(bool v) noexcept {
    flags_ &= ~Normals;
    if (v) flags_ |= Normals;
  }

  bool isPolygons() const noexcept { return flags_ & Polygons; }
  void isPolygons(bool v) noexcept {
    flags_ &= ~Polygons;
    if (v) flags_ |= Polygons;
  }

  value_type flags_;
} static flags;

inline void RenderPrimitiveShaderUi(PrimitiveShader* shader,
                                    const OpenGLApi* gl) noexcept {
  if (!ImGui::TreeNode("Primitive shader")) return;

  ImGui::Text("Render count: %lu", shader->renderCount());

  if (ImGui::CheckboxFlags("Normals", &flags.flags_, Flags::Normals)) {
    ShaderGuard sg(shader);
    shader->normals(flags.isNormals());
  }

  ImGui::SameLine();

  if (ImGui::CheckboxFlags("Polygons", &flags.flags_, Flags::Polygons))
    gl->polygonMode(GL_FRONT_AND_BACK, flags.isPolygons() ? GL_LINE : GL_FILL);

  ImGui::SeparatorText("Lighting");

  ImGui::PushID("PrimitiveShaderAmbientColor");
  auto color = shader->lightColor.value();
  std::array<f32, 3> colorArr = {color.r, color.g, color.b};
  if (ImGui::ColorEdit3("Ambient Color", colorArr.data())) {
    ShaderGuard guard(shader);
    shader->lightColor = glm::vec3(colorArr[0], colorArr[1], colorArr[2]);
  }
  ImGui::PopID();

  ImGui::PushID("PrimitiveShaderDiffuseMin");
  ImGui::PushItemWidth(UiHalfWidth - UiSpacing * 0.5);
  auto diffuseMin = shader->diffuseMin.value();
  if (ImGui::SliderFloat("", &diffuseMin, 0, 1, "Diffuse Min %.2f")) {
    ShaderGuard guard(shader);
    shader->diffuseMin = diffuseMin;
  }
  ImGui::PopID();

  ImGui::SameLine();

  ImGui::PushID("PrimitiveShaderAmbientStr");
  auto ambientStr = shader->ambientStr.value();
  if (ImGui::SliderFloat("", &ambientStr, 0, 1, "Ambient Str %.2f")) {
    ShaderGuard guard(shader);
    shader->ambientStr = ambientStr;
  }
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushItemWidth(UiHalfWidth - UiSpacing * 0.5);
  ImGui::PushID("PrimitiveShaderLightDir");
  auto lightDir = shader->lightDir.value();
  auto azimuth = std::atan2(lightDir.y, lightDir.x);
  auto elevation = std::asin(lightDir.z);
  bool updateLightDir = false;
  ImGui::PushID("PrimitiveShaderLightDirAzimuth");
  updateLightDir =
      ImGui::SliderAngle("", &azimuth, -180.0f, 180.0f, "Azimuth %.2f") ||
      updateLightDir;
  ImGui::PopID();

  ImGui::SameLine();

  ImGui::PushID("PrimitiveShaderLightDirElevation");
  updateLightDir =
      ImGui::SliderAngle("", &elevation, -90.0f, 90.0f, "Elevation %.2f") ||
      updateLightDir;
  ImGui::PopID();
  ImGui::PopItemWidth();

  if (updateLightDir) {
    ShaderGuard guard(shader);
    shader->lightDir = glm::vec3(cos(elevation) * cos(azimuth), sin(elevation),
                                 cos(elevation) * sin(azimuth));
  }
  ImGui::PopID();

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_PRIMITIVE_SHADER_UI_H_
