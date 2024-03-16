#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "absl/log/log.h"
#include "uinta/mesh.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/primitive.h"
#include "uinta/texture.h"
#include "uinta/utils/viewport_change.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Platform;

class DemoScene : public Scene {
 public:
  explicit DemoScene(const Engine* engine) noexcept
      : Scene(engine, Layer::Simulation) {
    std::array<Vertex, 4> vertices = {
        Vertex({0.5f, 0.5f, -1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0},
               {1.0f, 1.0f}),
        Vertex({0.5f, -0.5f, -1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0},
               {1.0f, 0.0f}),
        Vertex({-0.5f, -0.5f, -1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0},
               {0.0f, 0.0f}),
        Vertex({-0.5f, 0.5f, -1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0},
               {0.0f, 1.0f}),
    };
    VboGuard vbg(&vbo_);
    vbo_.bufferData(vertices.data(), vertices.size() * sizeof(f32),
                    GL_STATIC_DRAW);
    std::array<u32, 6> indices = {0, 1, 3, 1, 2, 3};
    Mesh mesh(vertices, indices);

    auto vsize = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
    VaoGuard vag(&vao_);
    vbo_.bufferData(mesh.vertices().data(), vsize, GL_STATIC_DRAW);
    vao_.ebo(mesh.elements());

    if (auto status = texture_.fromFile("wall.jpg"); !status.ok()) {
      LOG(FATAL) << status.message();
      return;
    }

    ShaderGuard sg(&shader_);
    shader_.linkAttributes(&vao_);
    shader_.lightDir = glm::vec3(0, 0, -1);
    shader_.uv();
  }

  void render(const EngineState& state) noexcept override {
    Scene::render(state);
    VaoGuard vg(&vao_);
    ShaderGuard sg(&shader_);
    TextureGuard tg(&texture_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    Scene::onViewportSizeChange(event);
    ShaderGuard sg(&shader_);
    shader_.projection = glm::perspective(45.0f, event.aspect(), 0.01f, 1.0f);
  }

 private:
  Texture texture_;
  PrimitiveShader shader_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
