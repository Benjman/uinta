#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/input.hpp>
#include <uinta/math/map.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

void processInput(TargetCamera&, const RunnerState&, const InputState&);
void updatePosition(TargetCamera&);

}  // namespace uinta

void uinta::update(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  processInput(cam, state, input);
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

void uinta::processInput(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  float scale = 1;
  if (isKeyDown(input, KEY_LEFT_CONTROL)) scale = 5;
  if (isKeyDown(input, KEY_LEFT_ALT)) scale = 0.1;

  glm::vec3 translate(0);
  if (isKeyDown(input, cam.config.translateForward))
    translate += glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateBackward))
    translate += -glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateLeft))
    translate += -glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateRight))
    translate += glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.translateSpeedKeyboard;
  if (isMouseButtonDown(input, cam.config.translateMouse) && (input.cursordx || input.cursordy))
    translate += -(glm::normalize(WORLD_HORIZONTAL * getRight(cam.angle)) * input.cursordx -
                   glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * input.cursordy) *
                 cam.config.translateSpeedMouse;

  float angleDelta = 0;
  if (isKeyDown(input, cam.config.angleLeft)) angleDelta += cam.config.angleSpeedKeyboard;
  if (isKeyDown(input, cam.config.angleRight)) angleDelta += -cam.config.angleSpeedKeyboard;
  if (isMouseButtonDown(input, cam.config.angleMouse) && input.cursordx)
    angleDelta += input.cursordx * cam.config.angleSpeedMouse;

  float distDelta = 0;
  if (isKeyDown(input, cam.config.distUp)) distDelta += -cam.config.distSpeedKeyboard;
  if (isKeyDown(input, cam.config.distDown)) distDelta += cam.config.distSpeedKeyboard;
  // TODO move speed should be proportional to distance. Closer moves slower, farther moves faster.
  if (isMouseScrolled(input)) distDelta += input.scrolldy * -cam.config.distSpeedMouse;

  float pitchDelta = 0;
  if (isKeyDown(input, cam.config.pitchUp)) pitchDelta += cam.config.pitchSpeedKeyboard;
  if (isKeyDown(input, cam.config.pitchDown)) pitchDelta += -cam.config.pitchSpeedKeyboard;
  if (isMouseButtonDown(input, cam.config.pitchMouse) && input.cursordy)
    pitchDelta += input.cursordy * cam.config.pitchSpeedMouse;

  cam.angle += angleDelta * state.delta * scale;
  cam.dist += distDelta * state.delta * scale;
  cam.pitch += pitchDelta * state.delta * scale;
  cam.target += translate * state.delta * scale;

  if (isFlagSet(CAMERA_DIST_LIMIT, cam.config.flags)) cam.dist = clamp(cam.dist.target, cam.config.distMin, cam.config.distMax);
  if (isFlagSet(CAMERA_PITCH_LIMIT, cam.config.flags))
    cam.pitch = clamp(cam.pitch.target, cam.config.pitchMin, cam.config.pitchMax);
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
