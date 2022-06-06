#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <uinta/input.hpp>
#include <uinta/math.hpp>
#include <uinta/runner.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace uinta {

const inline glm::vec3 WORLD_UP      = glm::vec3(0.0, 0.0, 1.0);
const inline glm::vec3 WORLD_RIGHT   = glm::vec3(1.0, 0.0, 0.0);
const inline glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 1.0, 0.0);

struct CameraConfig {
  float fov             = 40.0;
  float nearPlane       = 0.1;
  float farPlane        = 1000.0;
  float yOffset         = 7.0; // TODO use
  float minZoom         = 1.0;
  float maxZoom         = 75.0;
  float minPitch        = 26.0;
  float maxPitch        = 55.0;
  float rotateAgility   = 10.0;
  float zoomAgility     = 8.0;
  float startZoom       = 50.0;
  float startAngle      = -45.0;
  float dragSensitivity = 0.00125;
  float keyMoveSpeed    = 1.0;
  float zoomSensitivity = 5.0;
  float yawSensitivity  = 1.0;
  float cinRotateSpeed  = 0.75;

  glm::vec3 startTarget = glm::vec3(-1.5, 0.0, 1.5);

  CameraConfig() = default;

  CameraConfig(const CameraConfig &other);

  CameraConfig &operator=(const CameraConfig &other);
};

struct CameraControls {
  const CameraConfig &config;

  CameraControls(const CameraConfig &config) : config(config) {}

  float zoom(const RunnerState &state) { return state.input.scrolldy * config.zoomSensitivity; }

  float goRight(const RunnerState &state) { return state.input.isKeyDown(KEY_D) || state.input.isKeyDown(KEY_RIGHT); }

  float goLeft(const RunnerState &state) { return state.input.isKeyDown(KEY_A) || state.input.isKeyDown(KEY_LEFT); }

  float goForward(const RunnerState &state) { return state.input.isKeyDown(KEY_W) || state.input.isKeyDown(KEY_UP); }

  float goBackward(const RunnerState &state) { return state.input.isKeyDown(KEY_S) || state.input.isKeyDown(KEY_DOWN); }

  float rotation(const RunnerState &state) {
    if (state.input.isKeyDown(KEY_C))
      return config.cinRotateSpeed;
    if (state.input.isKeyDown(KEY_V))
      return -config.cinRotateSpeed;
    if (state.input.isMouseButtonDown(MOUSE_BUTTON_MIDDLE))
      return state.input.cursordx * config.yawSensitivity;
    return 0.0;
  }

  bool isDragging(const RunnerState &state) { return state.input.isMouseButtonDown(MOUSE_BUTTON_RIGHT); }

  bool reset(const RunnerState &state) { return state.input.isKeyPressed(KEY_R); }

  glm::vec2 dragFactor(const RunnerState &state) {
    return glm::vec2(-state.input.cursordx, -state.input.cursordy) * glm::vec2(config.dragSensitivity);
  }

  glm::vec2 movements(const RunnerState &state) {
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

  glm::vec2 moveDirection(const RunnerState &state, const CameraConfig &config) {
    if (isDragging(state))
      return dragFactor(state);
    return movements(state);
  }
};

struct Camera {
  CameraConfig config;
  CameraControls controls;

  glm::mat4 view     = glm::mat4(1.0);
  glm::vec3 position = glm::vec3(0.0);
  float pitch, yaw;

  SmoothVec3 target;
  SmoothFloat angle;
  SmoothFloat dist;

  Camera(const CameraConfig &config = CameraConfig());

  void tick(const RunnerState &state);
};

} // namespace uinta

#endif // UINTA_CAMERA2D_HglfwSetWindowSizeCallback
