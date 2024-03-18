#include "uinta/scenes/debug.h"

#include "uinta/component.h"
#include "uinta/scenes/axis_viewer.h"
#include "uinta/scenes/grid.h"

namespace uinta {

class ViewMatrixUpdater : public NewFrameComponent {
 public:
  explicit ViewMatrixUpdater(DebugScene* scene) noexcept
      : camera_(&scene->camera()), shader_(&scene->shader()) {}

  void update(const EngineState&) noexcept {
    if (comparitor_ != camera_) {
      comparitor_.position = camera_->position();
      comparitor_.angle = camera_->angle();
      comparitor_.dist = camera_->dist();
      comparitor_.pitch = camera_->pitch();

      ShaderGuard sg(shader_);
      shader_->view = camera_->viewMatrix();
    }
  }

 private:
  struct Comparitor {
    glm::vec3 position;
    SmoothFloat angle = SmoothFloat(8);
    SmoothFloat dist = SmoothFloat(8);
    SmoothFloat pitch = SmoothFloat(8);

    bool operator!=(const Camera* camera) {
      return position != camera->position() && angle != camera->angle() &&
             dist != camera->dist() && pitch != camera->pitch();
    }
  } comparitor_;
  const Camera* camera_;
  PrimitiveShader* shader_;
};

DebugScene::DebugScene(Layer layer, const OpenGLApi* gl) noexcept
    : Scene(layer), shader_(gl), gl_(gl) {
  addScene<AxisViewer>(&camera_);
  addScene<Grid>(15, &camera_);
  components().add<ViewMatrixUpdater>(this);
}

}  // namespace uinta
