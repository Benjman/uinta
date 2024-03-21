#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TREES_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TREES_SCENE_H_

#include <imgui.h>

#include "absl/random/random.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/flags.h"
#include "uinta/gl.h"
#include "uinta/math/defs.h"
#include "uinta/platform.h"
#include "uinta/scene/scene.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vbo.h"

namespace uinta {

static auto TreeCount = 20;

class TreeScene : public Scene {
 public:
  explicit TreeScene(Scene* parent) noexcept
      : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(parent->findSystem<BasicShaderManager>().value_or(nullptr)),
        input_(parent->engine()->input()) {
    if (!bsm_) {
      engine()->setStatusError(
          absl::NotFoundError("BasicShaderManager* not found."));
    }

    absl::BitGen bitgen;
    absl::uniform_real_distribution<f32> random(-7, 7);
    for (auto i = 0; i < TreeCount; i++)
      addTree(glm::translate(glm::mat4(1),
                             glm::vec3(random(bitgen), 0, random(bitgen))));
  }

  void preRender(time_t) noexcept override {
    if (input_->isMousePressed(MOUSE_BUTTON_LEFT)) {
      leftMouseDownPosition = {input_->cursorx(), input_->cursory()};
    } else if (input_->isMouseReleased(MOUSE_BUTTON_LEFT)) {
      auto cursorDelta =
          glm::length(glm::vec2(input_->cursorx(), input_->cursory()) -
                      leftMouseDownPosition) < epsilon_f;
      if (cursorDelta)
        addTree(glm::translate(glm::mat4(1), input_->cursorWorldPoint()));
    }
  }

  void onDebugUi() noexcept override {
    ImGui::Text("Render count: %zu", indexCount_);
    ImGui::PushItemWidth(50);
    ImGui::DragInt("Count", &TreeCount, 1, 1, 200);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Generate")) {
      absl::BitGen bitgen;
      absl::uniform_real_distribution<f32> random(-7, 7);
      vtxSegment_ = BufferSegment();
      idxSegment_ = BufferSegment();
      idxOffset_ = 0;
      indexCount_ = 0;
      for (auto i = 0; i < TreeCount; i++)
        addTree(glm::translate(glm::mat4(1),
                               glm::vec3(random(bitgen), 0, random(bitgen))));
    }
    auto flags = flags_.as<i32>();
    if (ImGui::CheckboxFlags("Sway", &flags, Flags::SwayMask))
      flags_.isSway(flags & Flags::SwayMask);
  }

  void render(time_t) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(bsm_->shader());
    VaoGuard vg(&vao_);

    bsm_->model(glm::scale(glm::mat4(1), glm::vec3(1)));
    bsm_->sway(flags_.isSway());
    bsm_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

  void addTree(glm::mat4 transform = glm::mat4(1)) noexcept {
    auto mesh = Mesh::Environment::Tree(&idxOffset_, transform);
    auto* data = mesh.vertices().data();
    auto size = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
    VboGuard vbg(&vbo_);
    vtxSegment_ = vbo_.bufferData(data, size, GL_STATIC_DRAW, vtxSegment_);

    VaoGuard vag(&vao_);
    idxSegment_ = vao_.ebo(mesh.elements(), idxSegment_);
    indexCount_ += mesh.elements().size();
    bsm_->linkAttributes(&vao_);
  }

 private:
  struct Flags final {
    using value_type = u8;

    static constexpr value_type SwayMask = 1 << 0;

    FlagsOperations(SwayMask);

    bool isSway() const noexcept { return flags_ & SwayMask; }
    void isSway(bool v) noexcept {
      flags_ &= ~SwayMask;
      if (v) flags_ |= SwayMask;
    }

    value_type flags_;
  } flags_;
  Vao vao_;
  Vbo vbo_;
  BufferSegment vtxSegment_;
  BufferSegment idxSegment_;
  size_t indexCount_ = 0;
  idx_t idxOffset_ = 0;
  glm::vec2 leftMouseDownPosition;
  BasicShaderManager* bsm_;
  const Input* input_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TREES_SCENE_H_
