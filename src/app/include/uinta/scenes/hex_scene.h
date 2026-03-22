#ifndef SRC_APP_INCLUDE_UINTA_SCENES_HEX_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_HEX_SCENE_H_

#include <imgui.h>

#include <algorithm>

#include "uinta/color.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/math/direction.h"
#include "uinta/math/hex.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

constexpr auto FillIndicies = 18;
constexpr auto OutlineIndicies = 7;
constexpr auto DrawFillMask = 1 << 14;
constexpr auto DrawOutlineMask = 1 << 15;

class HexScene final : public Scene {
 public:
  explicit HexScene(Scene* parent) noexcept
      : Scene(parent, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(findComponent<BasicShaderManager>().value_or(nullptr)),
        input_(parent->engine()->input()) {
    generateMesh();
  }

  void generateMesh() noexcept {
    hexCount_ = static_cast<i32>(group_.hexCount());
    outlineOffset_ = static_cast<i32>(FillIndicies * hexCount_);

    Hex origin(0, 0);
    auto radius = 2;
    auto hexagonSize = 1.0;
    auto fillPercentage = 1.0;
    HexGroup group(origin, radius, static_cast<f32>(hexagonSize),
                   static_cast<f32>(fillPercentage));
    Mesh fill = Mesh::Hexagon::Fill(&group);
    fill.translate(glm::vec3(0.01) * WorldUp);

    Mesh outline = Mesh::Hexagon::Outline(&group);
    outline.translate(glm::vec3(0.01) * WorldUp);

    VaoGuard vaoGuard(&vao_);
    VboGuard vboGuard(&vbo_);

    auto offset = vbo_.bufferData(
        fill.vertices().data(),
        static_cast<GLsizeiptr>(fill.vertices().size() * Vertex::ElementCount *
                                sizeof(f32)),
        GL_STATIC_DRAW);

    offset = vbo_.bufferData(
        outline.vertices().data(),
        static_cast<GLsizeiptr>(outline.vertices().size() *
                                Vertex::ElementCount * sizeof(f32)),
        GL_STATIC_DRAW, offset);

    bsm_->linkAttributes(&vao_);
  }

  void preRender(time_t /*unused*/) noexcept override {
    if (input_->isMouseReleased(MOUSE_BUTTON_LEFT)) {
      constexpr auto selectedColor = color::Red500;
      auto hex = group_.pointToHex(input_->cursorWorldPoint().xz());
      group_.setColor(hex, selectedColor);
      generateMesh();
    }
  }

  void onDebugUi() noexcept override {
    bool update = false;
    ImGui::Text("Render");
    ImGui::SameLine();
    ImGui::CheckboxFlags("Fill", &flags, DrawFillMask);
    ImGui::SameLine();
    ImGui::CheckboxFlags("Outline", &flags, DrawOutlineMask);
    constexpr auto HexMask = HexGroup::Flags::LayoutHexagonMask;
    constexpr auto HexStr = "Hexagon";
    constexpr auto RhombusMask = HexGroup::Flags::LayoutRhombusMask;
    constexpr auto RhombusStr = "Rhombus";
    if (ImGui::BeginCombo(
            "Layout", group_.flags.isLayoutHexagon() ? HexStr : RhombusStr)) {
      if (ImGui::Selectable(HexStr, (group_.flags & HexMask) != 0)) {
        group_.flags.isLayoutHexagon(update = true);
      }
      if (ImGui::Selectable(RhombusStr, (group_.flags & RhombusMask) != 0)) {
        group_.flags.isLayoutRhombus(update = true);
      }
      ImGui::EndCombo();
    }
    update |= ImGui::InputInt2("Origin", &group_.origin.q);
    update |= ImGui::InputInt("Group Radius", &group_.radius);
    update |= ImGui::SliderFloat("##hex_radius", &group_.size, 0.001, 4,
                                 "Radius %.2f");
    update |= ImGui::SliderFloat("##hex_fill", &group_.fillPercentage, 0, 1,
                                 "Fill %.2f");
    update |= ImGui::SliderFloat("##hex_linewidth", &lineWidth_, 0.01, 12,
                                 "Line Width %.2f");
    ImGui::Separator();
    if (update) {
      group_.radius = std::max(0, group_.radius);
      generateMesh();
    }
  }

  void render(time_t /*unused*/) noexcept override {
    ShaderGuard sg(bsm_->shader());
    VaoGuard vg(&vao_);
    LineWidthGuard lwg(lineWidth_);
    DepthTestGuard dtg;
    CullFaceGuard cfg;

    for (auto i = 0; i < hexCount_; i++) {
      if ((flags & DrawFillMask) != 0u) {
        bsm_->drawArrays(GL_TRIANGLE_STRIP, FillIndicies * i, FillIndicies);
      }
      if ((flags & DrawOutlineMask) != 0u) {
        bsm_->drawArrays(
            GL_LINE_STRIP,
            static_cast<GLint>(outlineOffset_ +
                               static_cast<GLint>(OutlineIndicies * i)),
            OutlineIndicies);
      }
    }
  }

 private:
  static constexpr i32 DrawFillMask = 1 << 14;
  static constexpr i32 DrawOutlineMask = 1 << 15;

  Vao vao_;
  Vbo vbo_;
  size_t outlineOffset_;
  u32 flags = DrawFillMask | DrawOutlineMask;
  f32 lineWidth_ = 2;
  i32 hexCount_ = 1;
  HexGroup group_;
  BasicShaderManager* bsm_;
  const Input* input_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_HEX_SCENE_H_
