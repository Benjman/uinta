#include <uinta/camera.hpp>

#include <glm/glm.hpp>

CameraConfig::CameraConfig(const CameraConfig& other) {
    *this = other;
}

CameraConfig& CameraConfig::operator=(const CameraConfig &other) {
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

Camera::Camera(const CameraConfig& config) :
    config(CameraConfig(config)),
    controls(this->config),
    target(config.startTarget),
    angle(config.startAngle, config.rotateAgility),
    dist(config.startZoom, config.zoomAgility) {
}


void Camera::tick(const RunnerState &state) {
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
    float yaw_rad = glm::radians(yaw);
    float yaw_side_rad = glm::radians(yaw + 90.0);
    float dx = move.y * speed * -glm::sin(yaw_rad);
    float dz = move.y * speed *  glm::cos(yaw_rad);
    float sideDx = move.x * speed *  glm::sin(yaw_side_rad);
    float sideDz = move.x * speed * -glm::cos(yaw_side_rad);
    target += glm::vec3(dx + sideDx, 0, dz + sideDz);
    target.tick(state.delta);

    yaw = std::fmod(360.0 - angle.current, 360.0);
    pitch = map0to1RangeClamped(dist.current, config.minZoom, config.maxPitch);
    pitch = dist.current;

    // update position
    float pitch_rad = glm::radians(pitch);
    float angle_rad = glm::radians(angle.current);
    float horizontal = dist.current * glm::cos(pitch_rad);
    float vertical = dist.current * glm::sin(pitch_rad);
    float x = target.x() + horizontal * glm::sin(angle_rad);
    float y = target.y() + vertical;
    float z = target.z() + horizontal * glm::cos(angle_rad);
    position = glm::vec3(x, y, z);
}
