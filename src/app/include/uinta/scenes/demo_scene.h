#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include "uinta/debug/debug_scene.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/texture_scene.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Engine* engine,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, layer), shader_(engine->gl()) {
    auto clearColor = glm::vec3(0.62, 0.67, 0.75);
    engine->gl()->clearColor(clearColor.r, clearColor.g, clearColor.b, 1.0);

    engine->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
        [&](const auto& event) {
          ShaderGuard guard(&shader_);
          shader_.projection =
              glm::perspective<f32>(45, event.aspect(), 0.1, 4);
        });

    addScene<DebugScene>();
    addScene<TextureScene>(&shader_);
  }

 private:
  struct DemoShader : Shader {
    explicit DemoShader(const OpenGLApi* gl) noexcept
        : Shader(
              {
                  {GL_VERTEX_SHADER, "shader.vs.glsl"},
                  {GL_FRAGMENT_SHADER, "shader.fs.glsl"},
              },
              gl) {}

    UniformMatrix4fv projection = UniformMatrix4fv("uProjection", this);
  } shader_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
