#include <spdlog/spdlog.h>

#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/runner/runner_state.hpp>

#include "uinta/math/direction.hpp"

namespace uinta {

const key_t FORWARD = KEY_E;
const key_t BACKWARD = KEY_D;
const key_t LEFT = KEY_S;
const key_t RIGHT = KEY_F;
const key_t PITCH_UP = KEY_Q;
const key_t PITCH_DOWN = KEY_A;
const key_t YAW_RIGHT = KEY_R;
const key_t YAW_LEFT = KEY_W;

const float TRANSLATE_SPEED = 3;
const float PITCH_SPEED = 45;
const float YAW_SPEED = 45;

void processInput(TargetCamera&, const RunnerState&);
void updatePosition(TargetCamera&);
}  // namespace uinta

void uinta::update(TargetCamera& cam, const RunnerState& state) {
  auto target = cam.target;
  auto pitch = cam.pitch;
  auto yaw = cam.yaw;
  processInput(cam, state);
  if (target != cam.target || pitch != cam.pitch || yaw != cam.yaw) updatePosition(cam);
}

glm::mat4 uinta::getViewMatrix(const TargetCamera& cam) {
  auto result = glm::rotate(glm::mat4(1), glm::radians(cam.pitch), WORLD_RIGHT);
  result = glm::rotate(result, glm::radians(cam.yaw), WORLD_UP);
  // result = glm::rotate(result, glm::radians(0.0f), WORLD_UP); // intended for roll
  return glm::translate(result, -cam.position);
}

void uinta::processInput(TargetCamera& cam, const RunnerState& state) {
  if (isKeyDown(state.input, FORWARD)) {
    auto translate = glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.yaw));
    cam.target += translate * TRANSLATE_SPEED * state.delta;
  }
  if (isKeyDown(state.input, BACKWARD)) {
    auto translate = glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.yaw));
    cam.target -= translate * TRANSLATE_SPEED * state.delta;
  }
  if (isKeyDown(state.input, LEFT)) {
    auto translate = getRight(cam.yaw) * WORLD_HORIZONTAL;
    cam.target -= translate * TRANSLATE_SPEED * state.delta;
  }
  if (isKeyDown(state.input, RIGHT)) {
    auto translate = getRight(cam.yaw) * WORLD_HORIZONTAL;
    cam.target += translate * TRANSLATE_SPEED * state.delta;
  }

  // TODO shouldn't these be inverted?
  if (isKeyDown(state.input, PITCH_UP)) cam.pitch -= PITCH_SPEED * state.delta;
  if (isKeyDown(state.input, PITCH_DOWN)) cam.pitch += PITCH_SPEED * state.delta;

  if (isKeyDown(state.input, YAW_LEFT)) cam.yaw -= YAW_SPEED * state.delta;
  if (isKeyDown(state.input, YAW_RIGHT)) cam.yaw += YAW_SPEED * state.delta;
}

void uinta::updatePosition(TargetCamera& cam) {
  cam.position = cam.target + WORLD_UP * 10.0f;
}
