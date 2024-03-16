#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_

#include "absl/log/log.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/mesh.h"
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

    std::array<Vertex, 4> vertices = {
        Vertex({-0.32, 0.45f, -1}, {0, 0, 1}, {1, 1, 1}, {0, 1}),
        Vertex({0.32, 0.45f, -1}, {0, 0, 1}, {1, 1, 1}, {1, 1}),
        Vertex({-0.32, -0.45f, -1}, {0, 0, 1}, {1, 1, 1}, {0, 0}),
        Vertex({0.32, -0.45f, -1}, {0, 0, 1}, {1, 1, 1}, {1, 0}),
    };

    Mesh mesh(vertices);

    VboGuard vbg(&vbo_);
    VaoGuard vag(&vao_);

    vbo_.bufferData(mesh.vertices().data(), sizeof(vertices), GL_STATIC_DRAW);

    constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE,
                                 Vertex::ElementCount * sizeof(GLfloat),
                                 0 * sizeof(GLfloat));
    constexpr Attribute normal(1, 3, GL_FLOAT, GL_FALSE,
                               Vertex::ElementCount * sizeof(GLfloat),
                               3 * sizeof(GLfloat));
    constexpr Attribute color(2, 3, GL_FLOAT, GL_FALSE,
                              Vertex::ElementCount * sizeof(GLfloat),
                              6 * sizeof(GLfloat));
    constexpr Attribute uv(3, 2, GL_FLOAT, GL_FALSE,
                           Vertex::ElementCount * sizeof(GLfloat),
                           9 * sizeof(GLfloat));
    vao_.linkAttribute(position);
    vao_.linkAttribute(normal);
    vao_.linkAttribute(color);
    vao_.linkAttribute(uv);
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
