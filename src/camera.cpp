#include <glm/glm.hpp>
#include <uinta/camera.hpp>
#include <uinta/math.hpp>

using namespace uinta;

CameraConfig::CameraConfig(const CameraConfig& other) { *this = other; }

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
      target(config.startTarget),
      angle(config.startAngle, config.rotateAgility),
      dist(config.startZoom, config.zoomAgility) {}

void Camera::tick(const RunnerState& state) {
  // angle
  float angleDelta = controls.rotation(state);
  angle -= angleDelta;
  angle.tick(state.delta);

  // zoom
  float targetZoom = dist.target;
  float zoomLevel = controls.zoom(state);
  targetZoom -= zoomLevel;
  targetZoom = glm::clamp(targetZoom, config.minZoom, config.maxZoom);

  dist = targetZoom;
  dist.tick(state.delta);

  // target
  glm::vec2 move = controls.moveDirection(state, config);
  float speed = dist.current;
  float yawRad = glm::radians(yaw);
  float yawSideRad = glm::radians(yaw + 90.0);

  float dx = move.y * speed * -glm::sin(yawRad);
  float dz = move.y * speed * glm::cos(yawRad);
  float sideDx = move.x * speed * glm::sin(yawSideRad);
  float sideDz = move.x * speed * -glm::cos(yawSideRad);

  target += glm::vec3(dx + sideDx, 0, dz + sideDz);
  target.tick(state.delta);

  // clamp values
  yaw = std::fmod(360.0 - angle.current, 360.0);
  pitch = map0to1RangeClamped(dist.current, config.minZoom, config.maxPitch);
  pitch = dist.current;

  // update position
  float pitchRad = glm::radians(pitch);
  float angleRad = glm::radians(angle.current);
  float horizontal = dist.current * glm::cos(pitchRad);
  float vertical = dist.current * glm::sin(pitchRad);

  float x = target.x() + horizontal * glm::sin(angleRad);
  float y = target.y() + vertical;
  float z = target.z() + horizontal * glm::cos(angleRad);

  position = glm::vec3(x, y, z);
}

void uinta::genViewMatrix(glm::mat4& viewMatrix, const Camera& camera) {
  genViewMatrix(viewMatrix, camera.position, camera.pitch, camera.yaw);
}
