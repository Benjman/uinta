#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include <string>

#include "uinta/engine/engine.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/primitive.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Platform;

class DemoScene : public Scene {
 public:
  explicit DemoScene(Scene* parent, Engine* engine) noexcept
      : Scene(parent, engine, Layer::Simulation),
        shader_(engine->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine->gl()),
        gl_(engine->gl()) {
    addSystem<PrimitiveShaderManager>(&shader_, engine->dispatchers());

    size_t idxOffset = 0;
    auto mesh = Mesh::Cube(&idxOffset);

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

  std::string name() const noexcept override { return "Demo scene"; }

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

 private:
  PrimitiveShader shader_;
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
