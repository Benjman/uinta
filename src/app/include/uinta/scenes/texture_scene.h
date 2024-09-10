#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_

#include "absl/log/log.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/texture.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class TextureScene : public Scene {
 public:
  explicit TextureScene(Scene* parent) noexcept
      : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
        bsm_(findSystem<BasicShaderManager>().value_or(nullptr)),
        texture_(GL_TEXTURE_2D, 0, 0, 0, 0, 0, engine()->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()) {
    if (!bsm_) {
      engine()->setStatusError(
          absl::NotFoundError("BasicShaderManager* not found."));
    }

    if (auto status = texture_.fromFile("texture.jpg"); !status.ok()) {
      LOG(FATAL) << status.message();
      return;
    }

    std::array<Vertex, 4> vertices = {
        Vertex({-0.28, 0.40f, -1}, {0, 0, 1}, {1, 1, 1}, {0, 1}),
        Vertex({0.28, 0.40f, -1}, {0, 0, 1}, {1, 1, 1}, {1, 1}),
        Vertex({-0.28, -0.40f, -1}, {0, 0, 1}, {1, 1, 1}, {0, 0}),
        Vertex({0.28, -0.40f, -1}, {0, 0, 1}, {1, 1, 1}, {1, 0}),
    };

    Mesh mesh(vertices);

    VboGuard vbg(&vbo_);
    VaoGuard vag(&vao_);

    vbo_.bufferData(mesh.vertices().data(), sizeof(vertices), GL_STATIC_DRAW);

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void render(time_t) noexcept override {
    ShaderGuard shaderGuard(bsm_->shader());
    VaoGuard vaoGuard(&vao_);
    TextureGuard textureGuard(&texture_);
    bsm_->uv();
    engine()->gl()->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

 private:
  BasicShaderManager* bsm_;
  Texture texture_;
  Vao vao_;
  Vbo vbo_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TEXTURE_SCENE_H_
