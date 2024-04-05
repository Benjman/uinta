#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_

#include "absl/log/log.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class TextureScene : public Scene {
 public:
  explicit TextureScene(Scene* parent, const Shader* shader) noexcept
      : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
        shader_(shader),
        texture_(GL_TEXTURE_2D, 0, 0, 0, 0, 0, engine()->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()) {
    if (auto status = texture_.fromFile("texture.jpg"); !status.ok()) {
      LOG(FATAL) << status.message();
      return;
    }

    std::array<f32, 16> vertices = {
        -0.32, 0.45,  0, 1,  // top-left
        0.32,  0.45,  1, 1,  // top-right
        -0.32, -0.45, 0, 0,  // bottom-left
        0.32,  -0.45, 1, 0,  // bottom-right
    };

    VboGuard vbg(&vbo_);
    VaoGuard vag(&vao_);
    vbo_.bufferData(vertices.data(), vertices.size() * sizeof(f32),
                    GL_STATIC_DRAW);
    vao_.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0});
    vao_.linkAttribute(
        {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat)});
  }

  void render(time_t) noexcept override {
    ShaderGuard shaderGuard(shader_);
    VaoGuard vaoGuard(&vao_);
    TextureGuard textureGuard(&texture_);
    engine()->gl()->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

 private:
  const Shader* shader_;
  Texture texture_;
  Vao vao_;
  Vbo vbo_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_
