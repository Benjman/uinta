#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "glm/ext/matrix_transform.hpp"
#include "uinta/engine_state.h"
#include "uinta/input.h"
#include "uinta/mesh.h"
#include "uinta/scenes/debug.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class DemoScene : public DebugScene {
 public:
  DemoScene() noexcept : DebugScene(Layer::Simulation) {
    auto mesh = fbx("pawn.fbx");
    std::for_each(mesh->at(0).vertices().begin(), mesh->at(0).vertices().end(),
                  [](auto& vertex) { vertex.color = glm::vec3(1); });
    vbo_.bufferData(mesh->at(0).vertices().data(),
                    mesh->at(0).vertices().size() * Vertex::ElementCount,
                    GL_STATIC_DRAW);
    vao_.indexBuffer(mesh->at(0).indices());
    indexCount_ = mesh->at(0).indices().size();

    vao_.linkAttribute(Primitive::PositionAttribute(&vbo_));
    vao_.linkAttribute(Primitive::NormalAttribute(&vbo_));
    vao_.linkAttribute(Primitive::ColorAttribute(&vbo_));
  }

  void render(const EngineState& state, const Input& input) noexcept override {
    DebugScene::render(state, input);

    DepthTestGuard dtg;
    CullFaceGuard cfg;

    auto model = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(0.001));

    ShaderGuard shaderGuard(&shader_);
    shader_.model = model;

    VaoGuard vaoGuard(&vao_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  size_t indexCount_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
