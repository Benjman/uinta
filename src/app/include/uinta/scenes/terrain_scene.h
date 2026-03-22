#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_

#include <imgui.h>

#include "uinta/color.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class TerrainScene : public Scene {
 public:
  explicit TerrainScene(Scene* parent) noexcept
      : Scene(parent, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(parent->findComponent<BasicShaderManager>().value_or(nullptr)),
        gl_(engine()->gl()) {
    if (bsm_ == nullptr) {
      engine()->setStatusError(
          absl::NotFoundError("BasicShaderManager* not found."));
    }

    generateMesh();
  }

  void generateMesh() noexcept {
    glm::mat4 transform(1);
    transform = glm::scale(transform, glm::vec3(size_.x, size_.y, size_.z));
    transform = glm::translate(transform, glm::vec3(0, -.5, 0));

    idx_t idxOffset = 0;
    auto mesh = Mesh::Cube(&idxOffset);
    mesh.color(color_);
    mesh.transformPositions(transform);

    indexCount_ = mesh.elements().size();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    static_cast<GLsizeiptr>(mesh.vertices().size() *
                                            Vertex::ElementCount * sizeof(f32)),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void onDebugUi() noexcept override {
    bool update = false;
    update |= ImGui::SliderFloat("##terrain_width", &size_.x, 0.1f, 100.0f,
                                 "Width %.2f");
    update |= ImGui::SliderFloat("##terrain_height", &size_.z, 0.1f, 100.0f,
                                 "Height %.2f");
    update |= ImGui::SliderFloat("##terrain_depth", &size_.y, 0.1f, 20.0f,
                                 "Depth %.2f");
    update |= ImGui::ColorEdit4("Color##terrain_color", &color_.r);
    ImGui::Separator();
    if (update) {
      generateMesh();
    }
  }

  void render(time_t /*unused*/) noexcept override {
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    ShaderGuard sg(bsm_->shader());
    VaoGuard vaoGuard(&vao_);
    gl_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_),
                      GL_UNSIGNED_INT, nullptr);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
  glm::vec3 size_ = glm::vec3(32, 0.5, 32);
  Color color_ = Color(85.0 / 255.0, 145.0 / 255.0, 95.0 / 255.0, 1.0);
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_
