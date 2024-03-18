#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_

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
      : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(parent->findSystem<BasicShaderManager>().value_or(nullptr)),
        gl_(engine()->gl()) {
    if (!bsm_) {
      engine()->setStatusError(
          absl::NotFoundError("BasicShaderManager* not found."));
    }

    glm::mat4 transform(1);
    transform = glm::scale(transform, glm::vec3(15, 1.5, 15));
    transform = glm::translate(transform, glm::vec3(0, -.5, 0));

    idx_t idxOffset = 0;
    auto mesh = Mesh::Cube(&idxOffset);
    mesh.color(color::Amber100);
    mesh.transformPositions(transform);

    indexCount_ = mesh.elements().size();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void render(time_t) noexcept override {
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    ShaderGuard sg(bsm_->shader());
    VaoGuard vaoGuard(&vao_);
    gl_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TERRAIN_SCENE_H_
