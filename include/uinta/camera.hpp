#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <uinta/camera/basic_camera.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/input.hpp>
#include <uinta/math/smooth_vec3.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

struct CameraConfig {
  float fov = 40.0;
  float nearPlane = 0.1;
  float farPlane = 1000.0;
  float yOffset = 7.0;  // TODO use
  float minZoom = 1.0;
  float maxZoom = 75.0;
  float minPitch = 26.0;
  float maxPitch = 55.0;
  float rotateAgility = 10.0;
  float zoomAgility = 8.0;
  float startZoom = 50.0;
  float startAngle = -45.0;
  float dragSensitivity = 0.00125;
  float keyMoveSpeed = 1.0;
  float zoomSensitivity = 5.0;
  float yawSensitivity = 1.0;
  float cinRotateSpeed = 0.75;

  glm::vec3 startTarget = glm::vec3(-1.5, 0.0, 1.5);

  CameraConfig() = default;

  CameraConfig(const CameraConfig& other);

  CameraConfig& operator=(const CameraConfig& other);
};

struct CameraControls {
  const CameraConfig& config;

  CameraControls(const CameraConfig& config) : config(config) {
  }

  float zoom(const RunnerState& state) {
    return state.input.scrolldy * config.zoomSensitivity;
  }

  float goRight(const RunnerState& state) {
    return isKeyDown(state.input, KEY_D) || isKeyDown(state.input, KEY_RIGHT);
  }

  float goLeft(const RunnerState& state) {
    return isKeyDown(state.input, KEY_A) || isKeyDown(state.input, KEY_LEFT);
  }

  float goForward(const RunnerState& state) {
    return isKeyDown(state.input, KEY_W) || isKeyDown(state.input, KEY_UP);
  }

  float goBackward(const RunnerState& state) {
    return isKeyDown(state.input, KEY_S) || isKeyDown(state.input, KEY_DOWN);
  }

  float rotation(const RunnerState& state) {
    if (isKeyDown(state.input, KEY_C)) return config.cinRotateSpeed;
    if (isKeyDown(state.input, KEY_V)) return -config.cinRotateSpeed;
    if (isMouseButtonDown(state.input, MOUSE_BUTTON_MIDDLE)) return state.input.cursordx * config.yawSensitivity;
    return 0.0;
  }

  bool isDragging(const RunnerState& state) {
    return isMouseButtonDown(state.input, MOUSE_BUTTON_RIGHT);
  }

  bool reset(const RunnerState& state) {
    return isKeyPressed(state.input, KEY_R);
  }

  glm::vec2 dragFactor(const RunnerState& state) {
    return glm::vec2(-state.input.cursordx, -state.input.cursordy) * glm::vec2(config.dragSensitivity);
  }

  glm::vec2 movements(const RunnerState& state) {
    glm::vec2 result(0);
    float speed = config.keyMoveSpeed * state.delta;
    if (goForward(state))
      result.y = -speed;
    else if (goBackward(state))
      result.y = speed;
    if (goRight(state))
      result.x = speed;
    else if (goLeft(state))
      result.x = -speed;
    return result;
  }

  glm::vec2 moveDirection(const RunnerState& state, const CameraConfig& config) {
    if (isDragging(state)) return dragFactor(state);
    return movements(state);
  }
};

struct Camera {
  CameraConfig config;
  CameraControls controls;

  glm::vec3 position{0};
  float pitch;
  float yaw;

  SmoothVec3 target;
  SmoothFloat angle;
  SmoothFloat dist;

  Camera(const CameraConfig& config = CameraConfig());
};

void update(Camera&, const RunnerState&);

}  // namespace uinta

#endif  // UINTA_CAMERA2D_HglfwSetWindowSizeCallback
