#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "absl/log/log.h"
#include "uinta/mesh.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/primitive.h"
#include "uinta/utils/viewport_change.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Platform;

class DemoScene : public Scene {
 public:
  explicit DemoScene(const Engine* engine) noexcept
      : Scene(engine, Layer::Simulation) {
    size_t idxOffset = 0;
    auto mesh = Mesh::Cube(&idxOffset);
    std::for_each(mesh.vertices().begin(), mesh.vertices().end(),
                  [](auto& vertex) { vertex.color = glm::vec3(1); });

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());
    indexCount_ = mesh.elements().size();

    ShaderGuard sg(&shader_);
    shader_.linkAttributes(&vao_);
    shader_.view = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
  }

  void render(const EngineState& state) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(&shader_);
    VaoGuard vg(&vao_);

    auto model = glm::rotate(
        glm::mat4(1), glm::radians(static_cast<f32>(state.runtime()) * 30),
        glm::normalize(glm::vec3(.25, 1, 0)));
    shader_.model = glm::scale(model, glm::vec3(0.5));

    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    Scene::onViewportSizeChange(event);
    ShaderGuard sg(&shader_);
    shader_.projection = glm::perspective(45.0f, event.aspect(), 0.01f, 1.0f);
  }

 private:
  PrimitiveShader shader_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  size_t indexCount_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
