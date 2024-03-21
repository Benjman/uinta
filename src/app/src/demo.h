#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include <algorithm>
#include <vector>

#include "uinta/engine_state.h"
#include "uinta/environment.h"
#include "uinta/input.h"
#include "uinta/primitive.h"
#include "uinta/scenes/debug.h"
#include "uinta/shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class DemoScene : public DebugScene {
 public:
  DemoScene() noexcept : DebugScene(Layer::Simulation) {
    size_t idxOffset = 0;
    auto tree1 = Environment::Tree(&idxOffset);
    auto tree2 = Environment::Tree(&idxOffset);
    std::for_each(
        tree2.vertices().begin(), tree2.vertices().end(), [](auto& v) {
          v.position = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 0)) *
                       glm::vec4(v.position, 1.0);
        });
    auto tree3 = Environment::Tree(&idxOffset);
    std::for_each(
        tree3.vertices().begin(), tree3.vertices().end(), [](auto& v) {
          v.position = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0)) *
                       glm::vec4(v.position, 1.0);
        });
    std::vector<Primitive> collection = {tree1, tree2, tree3};

    Primitive mesh(collection);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount,
                    GL_STATIC_DRAW);
    vao_.indexBuffer(mesh.indices());
    indexCount_ = mesh.indices().size();

    vao_.linkAttribute(Primitive::PositionAttribute(&vbo_));
    vao_.linkAttribute(Primitive::NormalAttribute(&vbo_));
    vao_.linkAttribute(Primitive::ColorAttribute(&vbo_));
  }

  void render(const EngineState& state, const Input& input) noexcept override {
    DebugScene::render(state, input);

    ShaderGuard sg(&shader_);
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    VaoGuard vg(&vao_);

    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  size_t indexCount_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
