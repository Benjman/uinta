#include <glm/glm.hpp>
#include <uinta/camera.hpp>
#include <uinta/math/map.hpp>
#include <uinta/math/smooth_vec3.hpp>

#include "./camera/target_camera.cpp"

using namespace uinta;

CameraConfig::CameraConfig(const CameraConfig& other) {
  *this = other;
}

CameraConfig& CameraConfig::operator=(const CameraConfig& other) {
  fov = other.fov;
  nearPlane = other.nearPlane;
  farPlane = other.farPlane;
  yOffset = other.yOffset;
  minZoom = other.minZoom;
  maxZoom = other.maxZoom;
  minPitch = other.minPitch;
  maxPitch = other.maxPitch;
  rotateAgility = other.rotateAgility;
  zoomAgility = other.zoomAgility;
  startZoom = other.startZoom;
  startAngle = other.startAngle;
  dragSensitivity = other.dragSensitivity;
  keyMoveSpeed = other.keyMoveSpeed;
  zoomSensitivity = other.zoomSensitivity;
  yawSensitivity = other.yawSensitivity;
  cinRotateSpeed = other.cinRotateSpeed;
  startTarget = other.startTarget;
  return *this;
}

Camera::Camera(const CameraConfig& config)
    : config(CameraConfig(config)),
      controls(this->config),
      target(10, config.startTarget),
      angle(config.startAngle, config.rotateAgility),
      dist(config.startZoom, config.zoomAgility) {
}

void uinta::update(Camera& camera, const RunnerState& state) {
  // angle
  auto angleDelta = camera.controls.rotation(state);
  camera.angle -= angleDelta;
  update(camera.angle, state.delta);

  // zoom
  auto targetZoom = camera.dist.target;
  auto zoomLevel = camera.controls.zoom(state);
  targetZoom -= zoomLevel;
  targetZoom = glm::clamp(targetZoom, camera.config.minZoom, camera.config.maxZoom);

  camera.dist = targetZoom;
  update(camera.dist, state.delta);

  // target
  glm::vec2 move = camera.controls.moveDirection(state, camera.config);
  auto speed = camera.dist.current;
  auto yawRad = glm::radians(camera.yaw);
  auto yawSideRad = glm::radians(camera.yaw + 90.0);

  auto dx = move.y * speed * -glm::sin(yawRad);
  auto dz = move.y * speed * glm::cos(yawRad);
  auto sideDx = move.x * speed * glm::sin(yawSideRad);
  auto sideDz = move.x * speed * -glm::cos(yawSideRad);

  camera.target += glm::vec3(dx + sideDx, 0, dz + sideDz);
  update(camera.target, state.delta);

  // clamp values
  camera.yaw = std::fmod(360.0 - camera.angle.current, 360.0);
  camera.pitch = map0to1RangeClamped(camera.dist.current, camera.config.minZoom, camera.config.maxPitch);
  camera.pitch = camera.dist.current;

  // update position
  auto pitchRad = glm::radians(camera.pitch);
  auto angleRad = glm::radians(camera.angle.current);
  auto horizontal = camera.dist.current * glm::cos(pitchRad);
  auto vertical = camera.dist.current * glm::sin(pitchRad);

  auto x = camera.target.x + horizontal * glm::sin(angleRad);
  auto y = camera.target.y + vertical;
  auto z = camera.target.z + horizontal * glm::cos(angleRad);

  camera.position = glm::vec3(x, y, z);
}
