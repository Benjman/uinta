#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "absl/log/log.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Platform;

class DemoScene : public Scene {
 public:
  explicit DemoScene(Scene* parent, Engine* engine) noexcept
      : Scene(parent, engine, Layer::Simulation),
        shader_(engine->gl()),
        texture_(GL_TEXTURE_2D, 0, 0, 0, 0, 0, engine->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine->gl()),
        gl_(engine->gl()) {
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

    engine->addListener<EngineEvent::ViewportSizeChange>(
        [&](const auto& event) {
          ShaderGuard guard(&shader_);
          shader_.projection =
              glm::perspective<f32>(45, event.aspect(), 0.1, 2);
        });
  }

  std::string name() const noexcept override { return "Demo scene"; }

  void render(const EngineState&) noexcept override {
    ShaderGuard shaderGuard(&shader_);
    VaoGuard vaoGuard(&vao_);
    TextureGuard textureGuard(&texture_);
    gl_->drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

 private:
  struct DemoShader : Shader {
    explicit DemoShader(const OpenGLApi* gl) noexcept
        : Shader(
              {
                  {GL_VERTEX_SHADER, "shader.vs.glsl"},
                  {GL_FRAGMENT_SHADER, "shader.fs.glsl"},
              },
              gl) {}

    UniformMatrix4fv projection = UniformMatrix4fv("uProjection", this);
  } shader_;
  Texture texture_;
  Vao vao_;
  Vbo vbo_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
