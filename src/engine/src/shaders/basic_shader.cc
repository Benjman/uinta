#include "uinta/shaders/basic_shader.h"

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"

namespace uinta {

BasicShaderManager::BasicShaderManager(Scene* scene) noexcept
    : shader_(scene->engine()->gl()) {
  if (auto opt = scene->findSystem<CameraManager>(); opt.has_value()) {
    opt.value()->addListener<CameraEvent::ProjectionMatrixUpdated>(
        [this](const auto& event) {
          projection_ = event.projection;
          flags_.isProjectionDirty(true);
        });

    opt.value()->addListener<CameraEvent::ViewMatrixUpdated>(
        [this](const auto& event) {
          view_ = event.view;
          flags_.isViewDirty(true);
        });
  }
}

}  // namespace uinta
