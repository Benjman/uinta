#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/fwd.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/input.hpp>
#include <uinta/math/map.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline void processInput(TargetCamera&, const RunnerState&, const InputState&);
inline void updatePosition(TargetCamera&);
inline f32 getAngleDelta(const InputState& input, const CameraConfig& config);
inline f32 getDistDelta(const InputState& input, const CameraConfig& config);
inline f32 getPitchDelta(const InputState& input, const CameraConfig& config);
inline glm::vec3 getTranslationDelta(const InputState& input, const TargetCamera& cam);

void updateCamera(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  processInput(cam, state, input);
  update(cam.target, state.delta);
  update(cam.angle, state.delta);
  update(cam.pitch, state.delta);
  update(cam.dist, state.delta);
  updatePosition(cam);
}

glm::mat4 getViewMatrix(const TargetCamera& cam) {
  auto delta = glm::rotate(glm::mat4(1), glm::radians(cam.pitch.current), WORLD_RIGHT);
  delta = glm::rotate(delta, glm::radians(cam.angle.current), WORLD_UP);
  delta = glm::translate(delta, -cam.position);
  return delta;
}

glm::mat4 getPerspectiveMatrix(const TargetCamera& cam, const Display& display) {
  glm::mat4 result;
  getPerspectiveMatrix(&result, cam, display);
  return result;
}

void getPerspectiveMatrix(glm::mat4* const ref, const TargetCamera& cam, const Display& display) {
  *ref = glm::perspective(cam.config.fov, display.aspectRatio, cam.config.nearPlane, cam.config.farPlane);
}

glm::mat4 getOrthographicMatrix(const TargetCamera& cam) {
  glm::mat4 result;
  getOrthographicMatrix(&result, cam);
  return result;
}

void getOrthographicMatrix(glm::mat4* const ref, const TargetCamera& cam) {
  constexpr auto size = 5.0f;
  *ref = glm::orthoLH(-size, size, -size, size, cam.config.nearPlane, cam.config.farPlane);
}

inline void processInput(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  auto scale = 1.0f;
  if (isKeyDown(input, KEY_LEFT_CONTROL)) scale = 5;
  if (isKeyDown(input, KEY_LEFT_ALT)) scale = 0.1;
  cam.angle += getAngleDelta(input, cam.config) * state.delta * scale;
  cam.dist += getDistDelta(input, cam.config) * state.delta * scale;
  cam.pitch += getPitchDelta(input, cam.config) * state.delta * scale;
  if (isFlagSet(CAMERA_DIST_LIMIT, cam.config.flags)) cam.dist = clamp(cam.dist.target, cam.config.distMin, cam.config.distMax);
  if (isFlagSet(CAMERA_PITCH_LIMIT, cam.config.flags))
    cam.pitch = clamp(cam.pitch.target, cam.config.pitchMin, cam.config.pitchMax);
  cam.target += getTranslationDelta(input, cam) * state.delta * scale;
}

inline void updatePosition(TargetCamera& cam) {
  const auto ar = glm::radians(cam.angle.current);
  const auto pr = glm::radians(cam.pitch.current);
  const auto ca = cos(ar);
  const auto cp = cos(pr);
  const auto sa = sin(ar);
  const auto sp = sin(pr);
  cam.position = {
      -sa * cp,
      sp,
      ca * cp,
  };
  cam.position *= cam.dist;
  cam.position += glm::vec3(cam.target) + cam.vertOffset * WORLD_UP;
}

inline f32 getAngleDelta(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.angleLeft)) delta += config.angleSpeedKeyboard;
  if (isKeyDown(input, config.angleRight)) delta += -config.angleSpeedKeyboard;
  if (isMouseButtonDown(input, config.angleMouse) && input.cursordx) delta += input.cursordx * config.angleSpeedMouse;
  return delta;
}

inline f32 getDistDelta(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.distUp)) delta += -config.distSpeedKeyboard;
  if (isKeyDown(input, config.distDown)) delta += config.distSpeedKeyboard;
  if (isMouseScrolled(input)) delta += input.scrolldy * -config.distSpeedMouse;
  return delta;
}

inline f32 getPitchDelta(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.pitchUp)) delta += config.pitchSpeedKeyboard;
  if (isKeyDown(input, config.pitchDown)) delta += -config.pitchSpeedKeyboard;
  if (isMouseButtonDown(input, config.pitchMouse) && input.cursordy) delta += input.cursordy * config.pitchSpeedMouse;
  return delta;
}

inline glm::vec3 getTranslationDelta(const InputState& input, const TargetCamera& cam) {
  glm::vec3 delta(0);
  if (isKeyDown(input, cam.config.translateForward))
    delta += glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateBackward))
    delta += -glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateLeft))
    delta += -glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.translateSpeedKeyboard;
  if (isKeyDown(input, cam.config.translateRight))
    delta += glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.translateSpeedKeyboard;
  if (isMouseButtonDown(input, cam.config.translateMouse) && (input.cursordx || input.cursordy))
    delta += -(glm::normalize(WORLD_HORIZONTAL * getRight(cam.angle)) * input.cursordx -
               glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * input.cursordy) *
             cam.config.translateSpeedMouse;
  return delta * calculateTranslationFactor(cam);
}

}  // namespace uinta
