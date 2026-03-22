#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include "uinta/debug/debug_scene.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/cube_scene.h"
#include "uinta/scenes/manifold_scene.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Engine* engine,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, layer) {
    auto clearColor = glm::vec3(58.0 / 255.0, 57.0 / 255.0, 66.0 / 255.0);
    engine->gl()->clearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);

    basicShader_ = addComponent<BasicShaderManager>();

    debugScene_ = addScene<DebugScene>();
    cubeScene_ = addScene<CubeScene>();
    manifoldScene_ = addScene<ManifoldScene>();
  }

 private:
  BasicShaderManager* basicShader_ = nullptr;

  DebugScene* debugScene_ = nullptr;
  CubeScene* cubeScene_ = nullptr;
  ManifoldScene* manifoldScene_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
