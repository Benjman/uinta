#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <uinta/input.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/runner/window.hpp>
#include <uinta/target_camera.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline void process_input(TargetCamera&, const RunnerState&, const InputState&);
inline void position(TargetCamera&);
inline f32 angle(const InputState& input, const CameraConfig& config);
inline f32 dist(const InputState& input, const CameraConfig& config);
inline f32 pitch(const InputState& input, const CameraConfig& config);
inline glm::vec3 translate(const InputState& input, const TargetCamera& cam);

void updateCamera(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  process_input(cam, state, input);
  update(cam.target, state.delta);
  update(cam.angle, state.delta);
  update(cam.pitch, state.delta);
  update(cam.dist, state.delta);
  position(cam);
}

glm::mat4 getViewMatrix(const TargetCamera& cam) {
  auto delta = glm::rotate(glm::mat4(1), glm::radians(cam.pitch.current), WORLD_RIGHT);
  delta = glm::rotate(delta, glm::radians(cam.angle.current), WORLD_UP);
  delta = glm::translate(delta, -cam.position);
  return delta;
}

glm::mat4 getPerspectiveMatrix(const TargetCamera& cam) {
  glm::mat4 result;
  getPerspectiveMatrix(&result, cam);
  return result;
}

void getPerspectiveMatrix(glm::mat4* const ref, const TargetCamera& cam) {
  // FIXME: The camera config shouldn't have knowledge of the window's aspect ratio. Make that an input parameter
  *ref = glm::perspective(cam.config.fov, cam.config.aspect_ratio, cam.config.near, cam.config.far);
}

glm::mat4 getOrthographicMatrix(const TargetCamera& cam) {
  glm::mat4 result;
  getOrthographicMatrix(&result, cam);
  return result;
}

void getOrthographicMatrix(glm::mat4* const ref, const TargetCamera& cam) {
  constexpr auto size = 5.0f;
  *ref = glm::orthoLH(-size, size, -size, size, cam.config.near, cam.config.far);
}

inline void process_input(TargetCamera& cam, const RunnerState& state, const InputState& input) {
  auto scale = 1.0f;
  if (isKeyDown(input, KEY_LEFT_CONTROL)) scale = 5;
  if (isKeyDown(input, KEY_LEFT_ALT)) scale = 0.1;
  cam.angle += angle(input, cam.config) * state.delta * scale;
  cam.dist += dist(input, cam.config) * state.delta * scale;
  cam.pitch += pitch(input, cam.config) * state.delta * scale;
  if (isFlagSet(TargetCamera::CAMERA_DIST_LIMIT, cam.flags))
    cam.dist = std::clamp(cam.dist.target, cam.config.dst_min, cam.config.dst_max);
  if (isFlagSet(TargetCamera::CAMERA_PITCH_LIMIT, cam.flags))
    cam.pitch = std::clamp(cam.pitch.target, cam.config.pitch_min, cam.config.pitch_max);
  cam.target += translate(input, cam) * state.delta * scale;
}

inline void position(TargetCamera& cam) {
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

inline f32 angle(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.angl_neg_k)) delta += config.angl_spd_k;
  if (isKeyDown(input, config.angl_pos_k)) delta += -config.angl_spd_k;
  if (isMouseButtonDown(input, config.angke_m) && input.cursordx) delta += input.cursordx * config.angl_spd_m;
  return delta;
}

inline f32 dist(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.dst_inc_k)) delta += -config.dst_spd_k;
  if (isKeyDown(input, config.dst_dec_k)) delta += config.dst_spd_k;
  if (isMouseScrolled(input)) delta += input.scrolldy * -config.dst_spd_m;
  return delta;
}

inline f32 pitch(const InputState& input, const CameraConfig& config) {
  f32 delta = 0;
  if (isKeyDown(input, config.pitch_pos_k)) delta += config.pitch_spd_k;
  if (isKeyDown(input, config.pitch_neg_k)) delta += -config.pitch_spd_k;
  if (isMouseButtonDown(input, config.pitch_m) && input.cursordy) delta += input.cursordy * config.pitch_m;
  return delta;
}

inline glm::vec3 translate(const InputState& input, const TargetCamera& cam) {
  glm::vec3 delta(0);
  if (isKeyDown(input, cam.config.forward_k))
    delta += glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.trnsl_spd_k;
  if (isKeyDown(input, cam.config.backward_k))
    delta += -glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * cam.config.trnsl_spd_k;
  if (isKeyDown(input, cam.config.left_k))
    delta += -glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.trnsl_spd_k;
  if (isKeyDown(input, cam.config.right_k))
    delta += glm::normalize(getRight(cam.angle) * WORLD_HORIZONTAL) * cam.config.trnsl_spd_k;
  if (isMouseButtonDown(input, cam.config.trnsl_m) && (input.cursordx || input.cursordy))
    delta += -(glm::normalize(WORLD_HORIZONTAL * getRight(cam.angle)) * input.cursordx -
               glm::normalize(WORLD_HORIZONTAL * getForward(cam.pitch, cam.angle)) * input.cursordy) *
             cam.config.trnsl_spd_m;
  return delta * calculateTranslationFactor(cam);
}

}  // namespace uinta
