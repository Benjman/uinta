#ifndef SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_

#include <string>

#include "uinta/debug/debug_scene.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"
#include "uinta/utils/fbx.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class FbxViewerScene : public Scene {
 public:
  explicit FbxViewerScene(Scene* parent, Engine* engine,
                          const DebugSceneParams* params,
                          std::string fbxPath = "model/pawn.fbx") noexcept
      : Scene(parent, engine, Layer::Simulation),
        shader_(params->shader),
        vbo_(GL_ARRAY_BUFFER, 0, engine->gl()) {
    auto mesh = loadFbx(fbxPath)->at(0);

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());
    indexCount_ = mesh.elements().size();

    shader_->linkAttributes(&vao_);
  }

  std::string name() const noexcept override { return "FbxViewerScene"; }

  void render(const EngineState&) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(shader_);
    VaoGuard vg(&vao_);
    shader_->model = glm::scale(glm::mat4(1), glm::vec3(0.001));
    shader_->sway(false);
    shader_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  PrimitiveShader* shader_;
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_H_
