#ifndef UINTA_TARGET_CAMERA_HPP
#define UINTA_TARGET_CAMERA_HPP

#include <glm/fwd.hpp>
#include <uinta/input.hpp>
#include <uinta/math/smooth_vec3.hpp>

namespace uinta {

struct RunnerState;

struct CameraConfig {
  f32 angl_spd_k = 90;       // Angle delta from keyboard input
  f32 angl_spd_m = 20;       // Angle delta from mouse input
  f32 aspect_ratio = 1;      // Aspect ratio from window (Needed to build projection matrix)
  f32 dst_max = 30;          // Maximum value when distance is clamped
  f32 dst_min = 0.2;         // Minimum value when distance is clamped
  f32 dst_spd_k = 15;        // Distance delta from keyboard input
  f32 dst_spd_m = 30;        // Distance delta from mouse input
  f32 far = 200;             // Far clipping plane
  f32 fov = 45;              // Field of view
  f32 near = 0.07;           // Near clipping plane
  f32 pitch_max = 90;        // Maximum value when pitch is clamped
  f32 pitch_min = 0.5;       // Minimum value when pitch is clamped
  f32 pitch_spd_k = 90;      // Pitch delta from keyboard input
  f32 pitch_spd_m = 20;      // Pitch delta from mouse input
  f32 spd_factor = 0.3;      // Factor at which translation speed scales proportional to distance
  f32 spd_factor_min = .66;  // Minimum speed factor
  f32 trnsl_spd_k = 10;      // Translation speed for keyboard input
  f32 trnsl_spd_m = 2.5;     // Translation speed for mouse input
  f32 ortho_size = 60;       // Size for the orthographic matrix boundaries

  input_key_t angl_neg_k = KEY_R;   // Keyboard input for negative angle delta
  input_key_t angl_pos_k = KEY_W;   // Keyboard input for positive angle delta
  input_key_t dst_dec_k = KEY_C;    // Keyboard input for decreasing distance
  input_key_t dst_inc_k = KEY_V;    // Keyboard input for increasing distance
  input_key_t pitch_neg_k = KEY_A;  // Keyboard input for negative pitch delta
  input_key_t pitch_pos_k = KEY_Q;  // Keyboard input for positive pitch delta
  input_key_t backward_k = KEY_D;   // Keyboard input for translation backward
  input_key_t forward_k = KEY_E;    // Keyboard input for translation forward
  input_key_t left_k = KEY_S;       // Keyboard input for translation left
  input_key_t right_k = KEY_F;      // Keyboard input for translation right

  mouse_button_t angke_m = MOUSE_BUTTON_RIGHT;  // Mouse input for angle
  mouse_button_t pitch_m = MOUSE_BUTTON_RIGHT;  // Mouse input for pitch
  mouse_button_t trnsl_m = MOUSE_BUTTON_LEFT;   // Mouse button for translate

  CameraConfig() {
  }

  CameraConfig(const CameraConfig& other) {
    *this = other;
  }
};

// disclaimer about euler angles and gimbal locking ...
class TargetCamera {
 public:
  static constexpr flag_t CAMERA_DIST_LIMIT = 1 << 0;
  static constexpr flag_t CAMERA_PITCH_LIMIT = 1 << 1;

  TargetCamera() = default;

  TargetCamera(const TargetCamera& other, const CameraConfig config = CameraConfig()) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    m_config = rhs.m_config;
    m_position = rhs.m_position;
    m_target = rhs.m_target;
    m_angle = rhs.m_angle;
    m_dist = rhs.m_dist;
    m_pitch = rhs.m_pitch;
    m_vert_offset = rhs.m_vert_offset;
    m_flags = rhs.m_flags;
    return *this;
  }

  void update(const RunnerState& state, const InputState& input);

  void process_input(const RunnerState& state, const InputState& input);

  glm::mat4 view_matrix() const noexcept;

  glm::mat4 perspective_matrix() const noexcept;

  glm::mat4 orthographic_matrix() const noexcept;

  void aspect_ratio(f32 v) noexcept {
    m_config.aspect_ratio = v;
  }

  f32 translation_factor() const noexcept {
    auto result = m_dist.current();
    result = std::abs(result);
    result = 1.3 * std::sqrt(result);
    result *= m_config.spd_factor;
    result = std::max(m_config.spd_factor_min, result);
    return result;
  }

  const CameraConfig& config() const noexcept {
    return m_config;
  }

  void config(const CameraConfig& config) noexcept {
    m_config = config;
  }

  const glm::vec3& position() const noexcept {
    return m_position;
  }

  void position(const glm::vec3& position) noexcept {
    m_position = position;
  }

  const SmoothVec3& target() const noexcept {
    return m_target;
  }

  void target(const SmoothVec3& target) noexcept {
    m_target = target;
  }

  void target(const glm::vec3& target) noexcept {
    m_target.x().target(target.x);
    m_target.y().target(target.y);
    m_target.z().target(target.z);
  }

  const SmoothFloat& angle() const noexcept {
    return m_angle;
  }

  void angle(const SmoothFloat& angle) noexcept {
    m_angle = angle;
  }

  void angle(f32 angle) noexcept {
    m_angle = angle;
  }

  const SmoothFloat& dist() const noexcept {
    return m_dist;
  }

  void dist(const SmoothFloat& dist) noexcept {
    m_dist = dist;
  }

  void dist(f32 dist) noexcept {
    m_dist = dist;
  }

  const SmoothFloat& pitch() const noexcept {
    return m_pitch;
  }

  void pitch(const SmoothFloat& pitch) noexcept {
    m_pitch = pitch;
  }

  void pitch(f32 pitch) noexcept {
    m_pitch = pitch;
  }

  f32 vert_offset() const noexcept {
    return m_vert_offset;
  }

  void vert_offset(f32 v) noexcept {
    m_vert_offset = v;
  }

  flags_t& flags() noexcept {
    return m_flags;
  }

 private:
  CameraConfig m_config;
  glm::vec3 m_position;
  SmoothVec3 m_target = {8, 0};
  SmoothFloat m_angle = {8, 0};
  SmoothFloat m_dist = {8, 1};
  SmoothFloat m_pitch = {8, 0};
  f32 m_vert_offset = 0;
  flags_t m_flags = CAMERA_DIST_LIMIT | CAMERA_PITCH_LIMIT;

  void update_angle(const RunnerState& state, const InputState& input) noexcept;

  void update_dist(const RunnerState& state, const InputState& input) noexcept;

  void update_pitch(const RunnerState& state, const InputState& input) noexcept;

  void update_translation(const RunnerState& state, const InputState& input) noexcept;

  void update_position(const RunnerState& state, const InputState& input) noexcept;
};

}  // namespace uinta

#endif  // UINTA_TARGET_CAMERA_HPP
