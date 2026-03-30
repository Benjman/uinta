#include "uinta/viewport/viewport_manager.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "uinta/app_config.h"
#include "uinta/engine/engine.h"

namespace uinta {

ViewportManager::ViewportManager(Engine* engine, AppConfig* appConfig) noexcept
    : appConfig_(appConfig) {
  assert(engine);
  assert(appConfig_);

  auto projStr =
      appConfig_->getString("viewport.projection").value_or("Perspective");
  config_.projection = toProjectionType(projStr);
  config_.fov = appConfig_->getFloat("viewport.fov").value_or(45.0f);
  config_.near = appConfig_->getFloat("viewport.near").value_or(0.1f);
  config_.far = appConfig_->getFloat("viewport.far").value_or(1000.0f);
  config_.orthoSize = appConfig_->getFloat("viewport.orthoSize").value_or(5.0f);

  engine->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [this](const auto& event) {
        aspect_ = event.aspect();
        dispatchProjectionUpdate();
      });
}

ViewportManager::~ViewportManager() noexcept {
  if (auto status = appConfig_->setFloat("viewport.far", config_.far);
      !status.ok()) {
    LOG(WARNING) << "Failed to cache viewport.far: " << status.message();
  }
  if (auto status = appConfig_->setFloat("viewport.fov", config_.fov);
      !status.ok()) {
    LOG(WARNING) << "Failed to cache viewport.fov: " << status.message();
  }
  if (auto status = appConfig_->setFloat("viewport.near", config_.near);
      !status.ok()) {
    LOG(WARNING) << "Failed to cache viewport.near: " << status.message();
  }
  if (auto status = appConfig_->setString("viewport.projection",
                                          toString(config_.projection));
      !status.ok()) {
    LOG(WARNING) << "Failed to cache viewport.projection: " << status.message();
  }
  if (auto status =
          appConfig_->setFloat("viewport.orthoSize", config_.orthoSize);
      !status.ok()) {
    LOG(WARNING) << "Failed to cache viewport.orthoSize: " << status.message();
  }
}

glm::mat4 ViewportManager::projectionMatrix() const noexcept {
  switch (config_.projection) {
    case ProjectionType::Perspective:
      return glm::perspective(glm::radians(config_.fov), aspect_, config_.near,
                              config_.far);
    case ProjectionType::Orthographic: {
      f32 halfWidth = config_.orthoSize * aspect_;
      f32 halfHeight = config_.orthoSize;
      return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                        config_.near, config_.far);
    }
    default:
      return {1.0f};
  }
}

void ViewportManager::projection(ProjectionType projection) noexcept {
  if (projection == config_.projection) {
    return;
  }
  config_.projection = projection;
  dispatchProjectionUpdate();
}

void ViewportManager::fov(f32 fov) noexcept {
  if (fov == config_.fov) {
    return;
  }
  config_.fov = fov;
  dispatchProjectionUpdate();
}

void ViewportManager::near(f32 near) noexcept {
  if (near == config_.near) {
    return;
  }
  config_.near = near;
  dispatchProjectionUpdate();
}

void ViewportManager::far(f32 far) noexcept {
  if (far == config_.far) {
    return;
  }
  config_.far = far;
  dispatchProjectionUpdate();
}

void ViewportManager::orthoSize(f32 orthoSize) noexcept {
  if (orthoSize == config_.orthoSize) {
    return;
  }
  config_.orthoSize = orthoSize;
  dispatchProjectionUpdate();
}

void ViewportManager::dispatchProjectionUpdate() noexcept {
  dispatchers_.dispatch<ViewportEvent::ProjectionMatrixUpdated>(
      ProjectionMatrixUpdateEvent{
          .viewport = this,
          .projection = projectionMatrix(),
          .aspect = aspect_,
      });
}

}  // namespace uinta
