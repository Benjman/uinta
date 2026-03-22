#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_

#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/math/direction.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class CubeScene : public Scene {
 public:
  explicit CubeScene(Scene* parent) noexcept
      : Scene(parent, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(findComponent<BasicShaderManager>().value_or(nullptr)),
        gl_(engine()->gl()) {
    auto mesh = Mesh::Cube();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    static_cast<GLsizeiptr>(mesh.vertices().size() *
                                            Vertex::ElementCount * sizeof(f32)),
                    GL_STATIC_DRAW);

    VaoGuard vg(&vao_);
    vao_.ebo(mesh.elements());

    indexCount_ = mesh.elements().size();

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void render(time_t delta) noexcept override {
    runtime_ += static_cast<f32>(delta);

    ShaderGuard sg(bsm_->shader());
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    VaoGuard vg(&vao_);

    constexpr f32 dist = 5;

    f32 x = std::sin(runtime_) * dist;
    f32 y = std::cos(runtime_) * dist;

    bsm_->view(glm::lookAt(glm::vec3(x, 0, y), glm::vec3(0), WorldUp));

    bsm_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_),
                       GL_UNSIGNED_INT, nullptr);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
  f32 runtime_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_
