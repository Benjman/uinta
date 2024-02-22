#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_

#include "uinta/camera.h"
#include "uinta/engine_signal.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"

namespace uinta {

class DebugScene : public Scene {
 public:
  explicit DebugScene(Layer,
                      const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  void tick(const EngineState& state, const Input& input) noexcept override {
    camera_.update(state, input);
  }

  void render(const EngineState& state, const Input&) noexcept override {
    ShaderGuard sg(&shader_);
    shader_.time = state.runtime();
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    ShaderGuard shaderGuard(&shader_);
    shader_.projection = camera_.perspectiveMatrix(event.aspect());
  }

  const Camera& camera() const noexcept { return camera_; }

  PrimitiveShader& shader() noexcept { return shader_; }

 protected:
  struct DebugCamera : Camera {
    DebugCamera() noexcept {
      CameraConfig cfg;
      cfg.vertOff = 0.5;
      cfg.dstMax = 20;
      config(cfg);
      dist(5);
      pitch(25);
      force();
    }
  } camera_;
  PrimitiveShader shader_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_DEBUG_H_
