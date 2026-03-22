#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include "uinta/debug/debug_scene.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/texture_scene.h"
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

    debugScene_ = addScene<DebugScene>();
    textureScene_ = addScene<TextureScene>();
  }

 private:
  BasicShaderManager* basicShader_ = nullptr;

  DebugScene* debugScene_ = nullptr;
  TextureScene* textureScene_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
