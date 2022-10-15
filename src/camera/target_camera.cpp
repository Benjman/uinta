#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/math/direction.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

const key_t FORWARD = KEY_E;
const key_t BACKWARD = KEY_D;
const key_t LEFT = KEY_S;
const key_t RIGHT = KEY_F;
const key_t PITCH_UP = KEY_Q;
const key_t PITCH_DOWN = KEY_A;
const key_t ANGLE_RIGHT = KEY_R;
const key_t ANGLE_LEFT = KEY_W;
const key_t DIST_UP = KEY_C;
const key_t DIST_DOWN = KEY_V;

const float ANGLE_SPEED = 45;
const float DIST_SPEED = 6;
const float PITCH_SPEED = 45;
const float TRANSLATE_SPEED = 2;

void processInput(TargetCamera&, const RunnerState&);
void updatePosition(TargetCamera&);

}  // namespace uinta

void uinta::update(TargetCamera& cam, const RunnerState& state) {
  processInput(cam, state);
  update(cam.target, state.delta);
  update(cam.angle, state.delta);
  update(cam.pitch, state.delta);
  update(cam.dist, state.delta);
  updatePosition(cam);
}

glm::mat4 uinta::getViewMatrix(const TargetCamera& cam) {
  auto result = glm::rotate(glm::mat4(1), glm::radians(cam.pitch.current), WORLD_RIGHT);
  result = glm::rotate(result, glm::radians(cam.angle.current), WORLD_UP);
  result = glm::translate(result, -cam.position);
  return result;
}

void uinta::processInput(TargetCamera& cam, const RunnerState& state) {
  float scale = 1;
  if (isKeyDown(state.input, KEY_LEFT_CONTROL)) scale = 5;
  if (isKeyDown(state.input, KEY_LEFT_ALT)) scale = 0.1;

  if (isKeyDown(state.input, FORWARD)) {
    auto translate = glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle));
    cam.target += translate * TRANSLATE_SPEED * state.delta * scale;
  }
  if (isKeyDown(state.input, BACKWARD)) {
    auto translate = glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle));
    cam.target -= translate * TRANSLATE_SPEED * state.delta * scale;
  }
  if (isKeyDown(state.input, LEFT)) {
    auto translate = getRight(cam.angle) * WORLD_HORIZONTAL;
    cam.target -= translate * TRANSLATE_SPEED * state.delta * scale;
  }
  if (isKeyDown(state.input, RIGHT)) {
    auto translate = getRight(cam.angle) * WORLD_HORIZONTAL;
    cam.target += translate * TRANSLATE_SPEED * state.delta * scale;
  }
  if (isKeyDown(state.input, ANGLE_LEFT)) cam.angle += ANGLE_SPEED * state.delta * scale;
  if (isKeyDown(state.input, ANGLE_RIGHT)) cam.angle -= ANGLE_SPEED * state.delta * scale;
  if (isKeyDown(state.input, PITCH_UP)) cam.pitch += PITCH_SPEED * state.delta * scale;
  if (isKeyDown(state.input, PITCH_DOWN)) cam.pitch -= PITCH_SPEED * state.delta * scale;
  if (isKeyDown(state.input, DIST_UP)) cam.dist -= DIST_SPEED * state.delta * scale;
  if (isKeyDown(state.input, DIST_DOWN)) cam.dist += DIST_SPEED * state.delta * scale;
}

void uinta::updatePosition(TargetCamera& cam) {
  auto ca = cos(glm::radians(cam.angle.current));
  auto cp = cos(glm::radians(cam.pitch.current));
  auto sa = sin(glm::radians(cam.angle.current));
  auto sp = sin(glm::radians(cam.pitch.current));
  cam.position = {
      -sa * cp,
      sp,
      ca * cp,
  };
  cam.position *= cam.dist;
  cam.position += glm::vec3(cam.target) + cam.vertOffset * WORLD_UP;
}
