#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_BASIC_SHADER_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_BASIC_SHADER_UI_H_

#include <imgui.h>

#include "uinta/debug/ui/utils.h"
#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"

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

inline void RenderBasicShaderUi(BasicShaderManager* bsm,
                                const OpenGLApi* gl) noexcept {
  if (!ImGui::TreeNode("Basic shader")) return;

  ImGui::Text("Render count: %lu", bsm->renderCount());

  if (ImGui::CheckboxFlags("Normals", &flags.flags_, Flags::Normals)) {
    ShaderGuard guard(bsm->shader());
    bsm->normals(flags.isNormals());
  }

  ImGui::SameLine();

  if (ImGui::CheckboxFlags("Polygons", &flags.flags_, Flags::Polygons))
    gl->polygonMode(GL_FRONT_AND_BACK, flags.isPolygons() ? GL_LINE : GL_FILL);

  ImGui::SeparatorText("Lighting");

  auto color = bsm->lightColor();
  if (ImGui::ColorEdit3("Ambient Color##BasicShaderAmbientColor",
                        glm::value_ptr<f32>(color))) {
    ShaderGuard guard(bsm->shader());
    bsm->lightColor(color);
  }

  ImGui::PushItemWidth(UiHalfWidth - UiSpacing * 0.5);
  auto diffuseMin = bsm->diffuseMin();
  if (ImGui::SliderFloat("##BasicShaderDiffuseMin", &diffuseMin, 0, 1,
                         "Diffuse Min %.2f")) {
    ShaderGuard guard(bsm->shader());
    bsm->diffuseMin(diffuseMin);
  }

  ImGui::SameLine();

  auto ambientStr = bsm->ambientStr();
  if (ImGui::SliderFloat("##BasicShaderAmbientStr", &ambientStr, 0, 1,
                         "Ambient Str %.2f")) {
    ShaderGuard guard(bsm->shader());
    bsm->ambientStr(ambientStr);
  }
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiHalfWidth - UiSpacing * 0.5);
  auto lightDir = bsm->lightDir();
  auto azimuth = std::atan2(lightDir.y, lightDir.x);
  auto elevation = std::asin(lightDir.z);
  bool updateLightDir = false;
  updateLightDir = ImGui::SliderAngle("##BasicShaderLightDirAzimuth", &azimuth,
                                      -180.0f, 180.0f, "Azimuth %.2f") ||
                   updateLightDir;

  ImGui::SameLine();

  updateLightDir =
      ImGui::SliderAngle("##BasicShaderLightDirElevation", &elevation, -90.0f,
                         90.0f, "Elevation %.2f") ||
      updateLightDir;
  ImGui::PopItemWidth();

  if (updateLightDir) {
    ShaderGuard guard(bsm->shader());
    bsm->lightDir(glm::vec3(cos(elevation) * cos(azimuth), sin(elevation),
                            cos(elevation) * sin(azimuth)));
  }

  ImGui::TreePop();
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_BASIC_SHADER_UI_H_
