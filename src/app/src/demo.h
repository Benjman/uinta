#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include <unordered_map>
#include <vector>

#include "absl/log/log.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  DemoScene() noexcept : Scene(Layer::Simulation) {
    std::vector<f32> vertices = {
        // positions  // uv coords
        0.5f,  0.5f,  1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f   // top left
    };
    VboGuard vbg(&vbo_);
    vbo_.bufferData(vertices.data(), vertices.size() * sizeof(f32),
                    GL_STATIC_DRAW);

    std::array<u32, 6> indexBuffer = {0, 1, 3, 1, 2, 3};
    VaoGuard vag(&vao_);
    vao_.ebo(indexBuffer);
    vao_.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0});
    vao_.linkAttribute(
        {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat)});

    if (auto status = texture_.fromFile("wall.jpg"); !status.ok()) {
      LOG(FATAL) << status.message();
      return;
    }
  }

  void render(const EngineState&) noexcept override {
    ShaderGuard shaderGuard(&shader_);
    VaoGuard vaoGuard(&vao_);
    TextureGuard textureGuard(&texture_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

 private:
  struct DemoShader : Shader {
    DemoShader() noexcept
        : Shader({
              {GL_VERTEX_SHADER, "shader.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader.fs.glsl"},
          }) {}
  } shader_;
  Texture texture_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
