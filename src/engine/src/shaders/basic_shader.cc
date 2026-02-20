#include "uinta/shaders/basic_shader.h"

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"

namespace uinta {

BasicShaderManager::BasicShaderManager(Scene* scene,
                                       CameraManager* cameraManager) noexcept
    : shader_(scene->engine()->gl()) {
  assert(cameraManager);
  cameraManager->addListener<CameraEvent::ProjectionMatrixUpdated>(
      [this](const auto& event) {
        projection_ = event.projection;
        flags_.isProjectionDirty(true);
      });
  cameraManager->addListener<CameraEvent::ViewMatrixUpdated>(
      [this](const auto& event) {
        view_ = event.view;
        flags_.isViewDirty(true);
      });
}

}  // namespace uinta
