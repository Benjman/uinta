#include "uinta/camera/camera_manager.h"

#include "uinta/app_config.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_serializer.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"

namespace uinta {

CameraManager::CameraManager(Scene* scene) noexcept
    : camera_(deserialize(scene->engine()->appConfig())),
      appConfig_(scene->engine()->appConfig()),
      input_(scene->engine()->input()) {
  orthoSize_ = appConfig_->getFloat("camera.orthoSize").value_or(5);

  scene->engine()->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [this](const auto& event) {
        glm::mat4 projection;
        aspect_ = event.aspect();
        switch (camera_.projection()) {
          case CameraProjection::Perspective:
            projection = camera_.perspectiveMatrix(aspect_);
            break;
          case CameraProjection::Orthographic:
            projection = camera_.orthographicMatrix(aspect_);
            break;
          default:
            LOG(ERROR) << "Unknown projection type!";
            break;
        }
        dispatchers_.dispatch<CameraEvent::ProjectionMatrixUpdated>(
            ProjectionMatrixUpdateEvent(&camera_, projection, aspect_));
      });
}

CameraManager::~CameraManager() noexcept {
  serialize(appConfig_, camera_.config());
}

}  // namespace uinta
