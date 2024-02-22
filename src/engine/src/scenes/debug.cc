#include "uinta/scenes/debug.h"

#include "uinta/engine.h"
#include "uinta/math/spatial.h"
#include "uinta/platform.h"
#include "uinta/scenes/axis_viewer.h"
#include "uinta/scenes/grid.h"
#include "uinta/shaders/primitive.h"
#include "uinta/system.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

class CameraManager : public System {
  struct DebugCamera;

 public:
  explicit CameraManager(PrimitiveShader* shader) noexcept : shader_(shader) {}

  void onNewFrame(const EngineState& state) noexcept override {
    camera_.update(state);
    if (comparitor_ != &camera_) {
      comparitor_.position = camera_.position();
      comparitor_.angle = camera_.angle();
      comparitor_.dist = camera_.dist();
      comparitor_.pitch = camera_.pitch();
      ShaderGuard sg(shader_);
      shader_->view = camera_.viewMatrix();
    }
  }

  const DebugCamera* camera() noexcept { return &camera_; }

 private:
  struct CameraComparitor {
    glm::vec3 position;
    SmoothFloat angle = SmoothFloat(8);
    SmoothFloat dist = SmoothFloat(8);
    SmoothFloat pitch = SmoothFloat(8);

    bool operator!=(const Camera* camera) noexcept {
      return position != camera->position() && angle != camera->angle() &&
             dist != camera->dist() && pitch != camera->pitch();
    }

   private:
  } comparitor_;
  struct DebugCamera : Camera {
    DebugCamera() noexcept {
      CameraConfig cfg;
      cfg.vertOff = 0.5;
      cfg.dstMax = 20;
      config(cfg);
      angle(-45);
      dist(5);
      pitch(25);
      force();
    }
  } camera_;
  PrimitiveShader* shader_;
};

DebugScene::DebugScene(Layer layer, const OpenGLApi* gl) noexcept
    : Scene(layer), shader_(gl), gl_(gl) {
  camera_ = addSystem<CameraManager>(&shader_)->camera();
  addScene<AxisViewer>(camera_);
  addScene<Grid>(15, camera_);
}

void DebugScene::onViewportSizeChange(
    const ViewportSizeChange& event) noexcept {
  ShaderGuard sg(&shader_);
  shader_.projection = camera_->perspectiveMatrix(event.aspect());
}

glm::vec3 DebugScene::cursorToWorldPoint(const Input& input) const noexcept {
  auto* window = engine()->platform()->window();
  glm::vec2 viewport(window->width(), window->height());
  glm::vec2 cursor(input.cursorx(), input.cursory());
  return getWorldPoint(*camera_, viewport, cursor);
}

}  // namespace uinta
