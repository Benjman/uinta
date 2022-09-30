#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/input/state.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {
void processInput(TargetCamera&, const RunnerState&);
}

void uinta::update(TargetCamera& cam, const RunnerState& state) {
  processInput(cam, state);

  float x = cam.dist * glm::sin(cam.yaw) + cam.target.x;
  float y = cam.dist * glm::sin(cam.pitch) + cam.target.y;
  float z = cam.dist * glm::cos(cam.pitch) + cam.target.z;
  cam.position = {x, y, z};
}

glm::mat4 uinta::getViewMatrix(const TargetCamera& cam) {
  return glm::lookAt(cam.position, glm::vec3(0), WORLD_UP);
}

void uinta::processInput(TargetCamera& cam, const RunnerState& state) {
  // translation
  float translationSensivity = 1;
  if (isKeyDown(state.input, KEY_E)) {
    cam.target += translationSensivity * state.delta * WORLD_FORWARD;
  }
  if (isKeyDown(state.input, KEY_D)) {
    cam.target -= translationSensivity * state.delta * WORLD_FORWARD;
  }
  if (isKeyDown(state.input, KEY_S)) {
    cam.target -= translationSensivity * state.delta * WORLD_RIGHT;
  }
  if (isKeyDown(state.input, KEY_F)) {
    cam.target += translationSensivity * state.delta * WORLD_RIGHT;
  }

  // pitch
  float pitchSensivity = 1;
  if (isKeyDown(state.input, KEY_W)) {
    cam.pitch -= pitchSensivity * state.delta;
  }
  if (isKeyDown(state.input, KEY_R)) {
    cam.pitch += pitchSensivity * state.delta;
  }

  // yaw
  float yawSensivity = 1;
  if (isKeyDown(state.input, KEY_C)) {
    cam.yaw -= yawSensivity * state.delta;
  }
  if (isKeyDown(state.input, KEY_V)) {
    cam.yaw += yawSensivity * state.delta;
  }

  // dist
  float distSensivity = 1;
  if (isKeyDown(state.input, KEY_Q)) {
    cam.dist -= distSensivity * state.delta;
  }
  if (isKeyDown(state.input, KEY_Z)) {
    cam.dist += distSensivity * state.delta;
  }
}
