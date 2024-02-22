#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_

#include "uinta/camera.h"
#include "uinta/engine_state.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"

namespace uinta {

class DebugScene : public Scene {
 public:
  explicit DebugScene(Layer,
                      const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  void preTick(const EngineState& state) noexcept override {
    Scene::preTick(state);
  }

  void tick(const EngineState& state) noexcept override { Scene::tick(state); }

  void postTick(const EngineState& state) noexcept override {
    Scene::postTick(state);
  }

  void preRender(const EngineState& state) noexcept override {
    Scene::preRender(state);
  }

  void render(const EngineState& state) noexcept override {
    Scene::render(state);
    ShaderGuard sg(&shader_);
    shader_.time = state.runtime();
  }

  void postRender(const EngineState& state) noexcept override {
    Scene::postRender(state);
  }

  const Camera* camera() const noexcept { return camera_; }

  glm::vec3 cursorToWorldPoint(const Input& input) const noexcept;

  void onViewportSizeChange(const ViewportSizeChange&) noexcept override;

  PrimitiveShader* shader() noexcept { return &shader_; }

 protected:
  PrimitiveShader shader_;
  const OpenGLApi* gl_;
  const Camera* camera_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_
