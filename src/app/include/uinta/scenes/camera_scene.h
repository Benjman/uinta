#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_

#include <manifold/manifold.h>

#include <glm/ext/matrix_transform.hpp>

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/manifold_mesh.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class CameraSceneShader : public Shader {
 public:
  explicit CameraSceneShader(const OpenGLApi* gl) noexcept
      : Shader({{GL_VERTEX_SHADER, "shader/camera.vs.glsl"},
                {GL_FRAGMENT_SHADER, "shader/camera.fs.glsl"}},
               gl) {
    ShaderGuard guard(this);
    model = glm::mat4(1);
    projection = glm::mat4(1);
    view = glm::mat4(1);
  }

  void linkAttributes(const Vao* vao) const noexcept override {
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
    vao->linkAttribute(position);
    vao->linkAttribute(normal);
    vao->linkAttribute(color);
    vao->linkAttribute(uv);
  }

  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv view = {"uView", this};
};

class CameraScene : public Scene {
 public:
  explicit CameraScene(Scene* parent) noexcept
      : Scene(parent, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        gl_(engine()->gl()),
        shader_(engine()->gl()) {
    auto* viewportManager = engine()->service<ViewportManager>();
    assert(viewportManager);
    viewportManager->addListener<ViewportEvent::ProjectionMatrixUpdated>(
        [this](const auto& event) {
          ShaderGuard guard(&shader_);
          shader_.projection = event.projection;
        });

    camera_ = engine()->service<CameraManager>();
    assert(camera_);
    camera_->addListener<CameraEvent::ViewMatrixUpdated>(
        [&](const ViewMatrixUpdateEvent& e) {
          ShaderGuard guard(&shader_);
          shader_.view = e.view;
        });

    auto sphere = manifold::Manifold::Sphere(1, 256);
    auto mesh = ToMesh(sphere);

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    static_cast<GLsizeiptr>(mesh.vertices().size() *
                                            Vertex::ElementCount * sizeof(f32)),
                    GL_STATIC_DRAW);

    VaoGuard vg(&vao_);
    vao_.ebo(mesh.elements());

    indexCount_ = mesh.elements().size();

    ShaderGuard sg(&shader_);
    shader_.linkAttributes(&vao_);
  }

  void render(time_t /*unused*/) noexcept override {
    ShaderGuard sg(&shader_);
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    VaoGuard vg(&vao_);

    gl_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_),
                      GL_UNSIGNED_INT, nullptr);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  const OpenGLApi* gl_;
  CameraManager* camera_;
  CameraSceneShader shader_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CUBE_SCENE_H_
