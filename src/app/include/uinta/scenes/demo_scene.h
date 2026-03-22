#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include <glm/ext/matrix_transform.hpp>

#include "uinta/camera/camera_manager.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/engine/cursor_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/math/direction.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/fbx_viewer_scene.h"
#include "uinta/scenes/manifold_scene.h"
#include "uinta/scenes/terrain_scene.h"
#include "uinta/scenes/text_scene.h"
#include "uinta/scenes/trees_scene.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Engine* engine,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, layer) {
    auto clearColor = glm::vec3(0.62, 0.67, 0.75);
    engine->gl()->clearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);

    basicShader_ = addComponent<BasicShaderManager>();
    camera_ = addComponent<CameraManager>();
    cusror_ = addComponent<CursorManager>();

    debugScene_ = addScene<DebugScene>();
    manifoldScene_ = addScene<ManifoldScene>();
    fbxViewerScene_ = addScene<FbxViewerScene>();
    terrainScene_ = addScene<TerrainScene>();
    treeScene_ = addScene<TreeScene>();
    textScene_ = addScene<TextScene>();
  }

  void render(time_t delta) noexcept override {
    runtime_ += delta;
    ShaderGuard shaderGuard(basicShader_->shader());
    basicShader_->view(glm::lookAt(
        static_cast<glm::vec3>(camera_->camera()->position()),
        static_cast<glm::vec3>(camera_->camera()->target()), WorldUp));
  }

 private:
  BasicShaderManager* basicShader_ = nullptr;
  CameraManager* camera_ = nullptr;
  CursorManager* cusror_ = nullptr;

  DebugScene* debugScene_ = nullptr;
  ManifoldScene* manifoldScene_ = nullptr;
  FbxViewerScene* fbxViewerScene_ = nullptr;
  TerrainScene* terrainScene_ = nullptr;
  TreeScene* treeScene_ = nullptr;
  TextScene* textScene_ = nullptr;

  time_t runtime_ = 0.0;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
