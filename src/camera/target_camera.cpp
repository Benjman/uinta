#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/math/map.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/utils/direction.hpp>

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

const key_t MOUSE_TRANSLATION = MOUSE_BUTTON_LEFT;
const key_t MOUSE_PITCH = MOUSE_BUTTON_MIDDLE;
const key_t MOUSE_ANGLE = MOUSE_BUTTON_MIDDLE;

const float MOUSE_TRANSLATION_SPEED = 0.5;
const float MOUSE_DIST_SPEED = 20;
const float MOUSE_PITCH_SPEED = 10;
const float MOUSE_ANGLE_SPEED = 10;

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

  // Keyboard input:
  glm::vec3 translate(0);
  if (isKeyDown(state.input, FORWARD))
    translate += glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * TRANSLATE_SPEED;
  if (isKeyDown(state.input, BACKWARD))
    translate += -glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * TRANSLATE_SPEED;
  if (isKeyDown(state.input, LEFT)) translate += -glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * TRANSLATE_SPEED;
  if (isKeyDown(state.input, RIGHT)) translate += glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * TRANSLATE_SPEED;
  if (isMouseButtonDown(state.input, MOUSE_TRANSLATION_SPEED) && (state.input.cursordx || state.input.cursordy))
    translate += -(glm::normalize(WORLD_HORIZONTAL * getRight(cam.angle)) * state.input.cursordx -
                   glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * state.input.cursordy) *
                 MOUSE_TRANSLATION_SPEED;

  float angleDelta = 0;
  if (isKeyDown(state.input, ANGLE_LEFT)) angleDelta += ANGLE_SPEED;
  if (isKeyDown(state.input, ANGLE_RIGHT)) angleDelta += -ANGLE_SPEED;
  if (isMouseButtonDown(state.input, MOUSE_ANGLE) && state.input.cursordx) angleDelta += state.input.cursordx * MOUSE_ANGLE_SPEED;

  float distDelta = 0;
  if (isKeyDown(state.input, DIST_UP)) distDelta += -DIST_SPEED;
  if (isKeyDown(state.input, DIST_DOWN)) distDelta += DIST_SPEED;
  if (isMouseScrolled(state.input)) distDelta += state.input.scrolldy * MOUSE_DIST_SPEED;

  float pitchDelta = 0;
  if (isKeyDown(state.input, PITCH_UP)) pitchDelta += PITCH_SPEED;
  if (isKeyDown(state.input, PITCH_DOWN)) pitchDelta += -PITCH_SPEED;
  if (isMouseButtonDown(state.input, MOUSE_PITCH) && state.input.cursordy) pitchDelta += state.input.cursordy * MOUSE_PITCH_SPEED;

  cam.angle += angleDelta * state.delta * scale;
  cam.dist += distDelta * state.delta * scale;
  cam.pitch += pitchDelta * state.delta * scale;
  cam.target += translate * state.delta * scale;

  cam.dist = clamp(cam.dist.target, 0.2, 10);
  cam.pitch = clamp(cam.pitch.target, 0, 90);
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
