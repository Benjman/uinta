#ifndef SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_SCENE_H_

#include <string>

#include "uinta/camera/camera_manager.h"
#include "uinta/color.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/utils/fbx.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class FbxViewerScene : public Scene {
 public:
  explicit FbxViewerScene(Scene* parent,
                          std::string fbxPath = "model/teacup.fbx") noexcept
      : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(findSystem<BasicShaderManager>().value_or(nullptr)) {
    if (!bsm_) {
      engine()->setStatusError(
          absl::NotFoundError("BasicShaderManager* not found."));
    }

    if (auto opt = findSystem<CameraManager>(); opt.has_value()) {
      auto* camera = opt.value()->camera();
      auto config = camera->config();
      config.vertOff = 1.0;
      camera->config(config);
      camera->angle(315);
      camera->dist(5);
      camera->pitch(45);
    }

    auto mesh = loadFbx(fbxPath)->at(0);
    mesh.color(color::Red500);

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    vao_.ebo(mesh.elements());
    indexCount_ = mesh.elements().size();

    bsm_->linkAttributes(&vao_);
  }

  void render(time_t) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(bsm_->shader());
    VaoGuard vg(&vao_);
    bsm_->sway(false);
    bsm_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;

  BasicShaderManager* bsm_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_FBX_VIEWER_SCENE_H_
