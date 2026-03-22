#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include "uinta/debug/debug_scene.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/cube_scene.h"
#include "uinta/scenes/manifold_scene.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Engine* engine, SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, layer), basicShader_(engine) {
    auto clearColor = glm::vec3(0.62, 0.67, 0.75);
    engine->service<const OpenGLApi>()->clearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);

    debugScene_ = addScene<DebugScene>();
    cubeScene_ = addScene<CubeScene>();
    manifoldScene_ = addScene<ManifoldScene>();
  }

  ~DemoScene() noexcept override { children().clear(); }

  void preRender(time_t delta) noexcept override { basicShader_.update(delta); }

  void render(time_t /*unused*/) noexcept override { ShaderGuard shaderGuard(basicShader_.shader()); }

 private:
  BasicShaderManager basicShader_;

  DebugScene* debugScene_ = nullptr;
  CubeScene* cubeScene_ = nullptr;
  ManifoldScene* manifoldScene_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
