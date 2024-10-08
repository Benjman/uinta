#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TREES_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TREES_H_

#include <imgui.h>

#include <string>

#include "absl/random/random.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/math/defs.h"
#include "uinta/math/spatial.h"
#include "uinta/platform.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"
#include "uinta/vbo.h"

namespace uinta {

static auto TreeCount = 20;

class TreeScene : public Scene {
 public:
  explicit TreeScene(Scene* parent, Engine* engine,
                     const DebugSceneParams* params) noexcept
      : Scene(parent, engine, Layer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine->gl()),
        shader_(params->shader),
        camera_(params->camera) {
    absl::BitGen bitgen;
    absl::uniform_real_distribution<f32> random(-7, 7);
    for (auto i = 0; i < TreeCount; i++)
      addTree(glm::translate(glm::mat4(1),
                             glm::vec3(random(bitgen), 0, random(bitgen))));
  }

  std::string name() const noexcept override { return "TreeScene"; }

  void tick(const EngineState& state) noexcept override {
    if (!state.isNewFrame()) return;

    if (state.isMousePressed(MOUSE_BUTTON_LEFT)) {
      leftMouseDownPosition = {state.input().cursorx(),
                               state.input().cursory()};
    } else if (state.isMouseReleased(MOUSE_BUTTON_LEFT)) {
      auto cursorDelta = glm::length(glm::vec2(state.input().cursorx(),
                                               state.input().cursory()) -
                                     leftMouseDownPosition) < epsilon_f;
      if (cursorDelta)
        addTree(
            glm::translate(glm::mat4(1), cursorToWorldPoint(state.input())));
    }
  }

  void onDebugUi(const EngineState&) noexcept override {
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
  }

  void render(const EngineState&) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(shader_);
    VaoGuard vg(&vao_);

    shader_->model = glm::scale(glm::mat4(1), glm::vec3(1));
    shader_->sway(true);
    shader_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
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
    shader_->linkAttributes(&vao_);
  }

  glm::vec3 cursorToWorldPoint(const Input& input) const noexcept {
    auto* window = engine()->platform()->window();
    glm::vec2 viewport(window->width(), window->height());
    glm::vec2 cursor(input.cursorx(), input.cursory());
    return getWorldPoint(*camera_, viewport, cursor);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  BufferSegment vtxSegment_;
  BufferSegment idxSegment_;
  size_t indexCount_ = 0;
  size_t idxOffset_ = 0;
  PrimitiveShader* shader_;
  const Camera* camera_;
  glm::vec2 leftMouseDownPosition;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TREES_H_
