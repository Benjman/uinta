#ifndef SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_

#include <string>

#include "uinta/debug/debug_scene.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"
#include "uinta/utils/fbx.h"
#include "uinta/utils/viewport_change.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class FbxViewerScene : public Scene {
 public:
  explicit FbxViewerScene(Scene* parent, const DebugSceneParams& params,
                          std::string fbxPath = "model/pawn.fbx") noexcept
      : Scene(parent, Layer::Simulation), shader_(params.shader) {
    auto mesh = loadFbx(fbxPath)->at(0);

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());
    indexCount_ = mesh.elements().size();

    ShaderGuard sg(shader_);
    shader_->linkAttributes(&vao_);
    shader_->view = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
    shader_->model = glm::scale(glm::mat4(1), glm::vec3(0.001));
  }

  void render(const EngineState& state) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(shader_);
    VaoGuard vg(&vao_);
    glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.0001));
    model = glm::rotate(model, static_cast<f32>(state.runtime()),
                        glm::vec3(0, 1, 0));
    shader_->model = model;
    shader_->sway(false);

    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    ShaderGuard sg(shader_);
    shader_->projection = glm::perspective(45.0f, event.aspect(), 0.01f, 1.0f);
  }

 private:
  PrimitiveShader* shader_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  size_t indexCount_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_
