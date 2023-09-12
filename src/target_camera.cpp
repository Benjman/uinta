#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <uinta/input.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/runner/window.hpp>
#include <uinta/target_camera.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

void TargetCamera::update(const RunnerState& state, const InputState& input) {
  process_input(state, input);
  update_position(state, input);
}

glm::mat4 TargetCamera::view_matrix() const noexcept {
  glm::mat4 result = glm::rotate(glm::mat4(1), glm::radians(m_pitch.current()), WORLD_RIGHT);
  result = glm::rotate(result, glm::radians(m_angle.current()), WORLD_UP);
  result = glm::translate(result, -m_position);
  return result;
}

glm::mat4 TargetCamera::perspective_matrix() const noexcept {
  // FIXME: The camera config shouldn't have knowledge of the window's aspect ratio. Make that an input parameter
  return glm::perspective(m_config.fov, m_config.aspect_ratio, m_config.near, m_config.far);
}

glm::mat4 TargetCamera::orthographic_matrix() const noexcept {
  return glm::ortho(-m_config.ortho_size, m_config.ortho_size, -m_config.ortho_size, m_config.ortho_size, m_config.near,
                    m_config.far);
}

void TargetCamera::process_input(const RunnerState& state, const InputState& input) {
  RunnerState state_copy = state;
  if (isKeyDown(input, KEY_LEFT_CONTROL)) state_copy.delta *= 5.0f;
  if (isKeyDown(input, KEY_LEFT_ALT)) state_copy.delta *= 0.1;

  update_angle(state_copy, input);
  m_angle.update(state.delta);

  update_pitch(state_copy, input);
  m_pitch.update(state.delta);

  state_copy.delta *= translation_factor();

  update_dist(state_copy, input);
  m_dist.update(state.delta);

  update_translation(state_copy, input);
  m_target.update(state.delta);

  update_position(state_copy, input);
}

void TargetCamera::update_position(const RunnerState& state, const InputState& input) noexcept {
  const auto ar = glm::radians(m_angle.current());
  const auto pr = glm::radians(m_pitch.current());
  const auto ca = cos(ar);
  const auto cp = cos(pr);
  const auto sa = sin(ar);
  const auto sp = sin(pr);
  m_position = {
      -sa * cp,
      sp,
      ca * cp,
  };
  m_position *= m_dist;
  m_position += glm::vec3(m_target) + m_vert_offset * WORLD_UP;
}

void TargetCamera::update_angle(const RunnerState& state, const InputState& input) noexcept {
  f32 delta = 0;
  if (isKeyDown(input, m_config.angl_neg_k)) delta += m_config.angl_spd_k;
  if (isKeyDown(input, m_config.angl_pos_k)) delta += -m_config.angl_spd_k;
  if (isMouseButtonDown(input, m_config.angke_m) && input.cursordx) delta += input.cursordx * m_config.angl_spd_m;
  m_angle += delta * state.delta;
}

void TargetCamera::update_dist(const RunnerState& state, const InputState& input) noexcept {
  f32 delta = 0;
  if (isKeyDown(input, m_config.dst_inc_k)) delta += -m_config.dst_spd_k;
  if (isKeyDown(input, m_config.dst_dec_k)) delta += m_config.dst_spd_k;
  if (isMouseScrolled(input)) delta += input.scrolldy * -m_config.dst_spd_m;
  m_dist += delta * state.delta;
  if (isFlagSet(TargetCamera::CAMERA_DIST_LIMIT, m_flags))
    m_dist = std::clamp(m_dist.target(), m_config.dst_min, m_config.dst_max);
  if (isFlagSet(TargetCamera::CAMERA_PITCH_LIMIT, m_flags))
    m_pitch = std::clamp(m_pitch.target(), m_config.pitch_min, m_config.pitch_max);
}

void TargetCamera::update_pitch(const RunnerState& state, const InputState& input) noexcept {
  f32 delta = 0;
  if (isKeyDown(input, m_config.pitch_pos_k)) delta += m_config.pitch_spd_k;
  if (isKeyDown(input, m_config.pitch_neg_k)) delta += -m_config.pitch_spd_k;
  if (isMouseButtonDown(input, m_config.pitch_m) && input.cursordy) delta += input.cursordy * m_config.pitch_spd_m;
  m_pitch += delta * state.delta;
}

void TargetCamera::update_translation(const RunnerState& state, const InputState& input) noexcept {
  glm::vec3 delta(0);
  if (isKeyDown(input, m_config.forward_k))
    delta += glm::normalize(WORLD_HORIZONTAL * getForward(m_pitch, m_angle)) * m_config.trnsl_spd_k;
  if (isKeyDown(input, m_config.backward_k))
    delta += -glm::normalize(WORLD_HORIZONTAL * getForward(m_pitch, m_angle)) * m_config.trnsl_spd_k;
  if (isKeyDown(input, m_config.left_k)) delta += -glm::normalize(getRight(m_angle) * WORLD_HORIZONTAL) * m_config.trnsl_spd_k;
  if (isKeyDown(input, m_config.right_k)) delta += glm::normalize(getRight(m_angle) * WORLD_HORIZONTAL) * m_config.trnsl_spd_k;
  if (isMouseButtonDown(input, m_config.trnsl_m) && (input.cursordx || input.cursordy))
    delta += -(glm::normalize(WORLD_HORIZONTAL * getRight(m_angle)) * input.cursordx -
               glm::normalize(WORLD_HORIZONTAL * getForward(m_pitch, m_angle)) * input.cursordy) *
             m_config.trnsl_spd_m;
  m_target += delta * state.delta;
}

}  // namespace uinta
