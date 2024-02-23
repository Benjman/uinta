#include "uinta/debug/debug_scene.h"

#include "uinta/camera/camera.h"
#include "uinta/camera/comparator.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine.h"
#include "uinta/math/spatial.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

class CameraManager : public System {
  struct DebugCamera;

 public:
  explicit CameraManager(PrimitiveShader* shader) noexcept : shader_(shader) {}

  void onNewFrame(const EngineState& state) noexcept override {
    camera_.update(state);
    if (comparator_ != &camera_) {
      comparator_ = camera_;
      ShaderGuard sg(shader_);
      shader_->view = camera_.viewMatrix();
    }
  }

  DebugCamera* camera() noexcept { return &camera_; }

 private:
  struct DebugCamera : Camera {
    DebugCamera() noexcept : Camera() {
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

  CameraComparator comparator_;
  PrimitiveShader* shader_;
};

DebugScene::DebugScene(Params params, Layer layer) noexcept
    : Scene(params.engine, layer),
      camera_(addSystem<CameraManager>(&shader_)->camera()),
      shader_(params.gl),
      gl_(params.gl),
      params_(params) {
  params_.camera = camera_;
  params_.shader = &shader_;
  addScene<DebugSceneUi>(this, params_);
}

glm::vec3 DebugScene::cursorToWorldPoint(const Input& input) const noexcept {
  auto* window = engine()->platform()->window();
  glm::vec2 viewport(window->width(), window->height());
  glm::vec2 cursor(input.cursorx(), input.cursory());
  return getWorldPoint(*camera_, viewport, cursor);
}

void DebugScene::onViewportSizeChange(
    const ViewportSizeChange& event) noexcept {
  ShaderGuard sg(&shader_);
  shader_.projection = camera_->perspectiveMatrix(event.aspect());
}

void DebugScene::render(const EngineState& state) noexcept {
  Scene::render(state);
  ShaderGuard sg(&shader_);
  shader_.time = state.runtime();
}

}  // namespace uinta
