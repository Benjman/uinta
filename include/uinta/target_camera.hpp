#ifndef UINTA_TARGET_CAMERA_HPP
#define UINTA_TARGET_CAMERA_HPP

#include <uinta/input/keys.hpp>
#include <uinta/math/smooth_float.hpp>
#include <uinta/math/smooth_vec3.hpp>

namespace uinta {

struct RunnerState;

struct CameraConfig {
  f32 angl_spd_k = 90;       // Angle delta from keyboard input
  f32 angl_spd_m = 20;       // Angle delta from mouse input
  f32 aspect_ratio = 1;      // Aspect ratio from window (Needed to build projection matrix)
  f32 dst_max = 30;          // Maximum value when distance is clamped
  f32 dst_min = 0.2;         // Minimum value when distance is clamped
  f32 dst_spd_k = 20;        // Distance delta from keyboard input
  f32 dst_spd_m = 60;        // Distance delta from mouse input
  f32 far = 200;             // Far clipping plane
  f32 fov = 45;              // Field of view
  f32 near = 0.07;           // Near clipping plane
  f32 pitch_max = 90;        // Maximum value when pitch is clamped
  f32 pitch_min = 0.5;       // Minimum value when pitch is clamped
  f32 pitch_spd_k = 90;      // Pitch delta from keyboard input
  f32 pitch_spd_m = 20;      // Pitch delta from mouse input
  f32 spd_factor = .45;      // Factor at which translation speed scales proportional to distance
  f32 spd_factor_min = .15;  // Minimum speed factor
  f32 trnsl_spd_k = 4;       // Translation speed for keyboard input
  f32 trnsl_spd_m = 2;       // Translation speed for mouse input

  input_key_t angl_neg_k = KEY_W;   // Keyboard input for negative angle delta
  input_key_t angl_pos_k = KEY_R;   // Keyboard input for positive angle delta
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

  CameraConfig& operator=(const CameraConfig& rhs) {
    angke_m = rhs.angke_m;
    angl_neg_k = rhs.angl_neg_k;
    angl_pos_k = rhs.angl_pos_k;
    angl_spd_k = rhs.angl_spd_k;
    angl_spd_m = rhs.angl_spd_m;
    aspect_ratio = rhs.aspect_ratio;
    backward_k = rhs.backward_k;
    dst_dec_k = rhs.dst_dec_k;
    dst_inc_k = rhs.dst_inc_k;
    dst_max = rhs.dst_max;
    dst_min = rhs.dst_min;
    dst_spd_k = rhs.dst_spd_k;
    dst_spd_m = rhs.dst_spd_m;
    far = rhs.far;
    forward_k = rhs.forward_k;
    fov = rhs.fov;
    left_k = rhs.left_k;
    near = rhs.near;
    pitch_neg_k = rhs.pitch_neg_k;
    pitch_m = rhs.pitch_m;
    pitch_max = rhs.pitch_max;
    pitch_min = rhs.pitch_min;
    pitch_spd_k = rhs.pitch_spd_k;
    pitch_spd_m = rhs.pitch_spd_m;
    pitch_pos_k = rhs.pitch_pos_k;
    right_k = rhs.right_k;
    spd_factor = rhs.spd_factor;
    spd_factor_min = rhs.spd_factor_min;
    trnsl_m = rhs.trnsl_m;
    trnsl_spd_k = rhs.trnsl_spd_k;
    trnsl_spd_m = rhs.trnsl_spd_m;
    return *this;
  }
};

// disclaimer about euler angles and gimbal locking ...
struct TargetCamera {
  static constexpr flag_t CAMERA_DIST_LIMIT = 1 << 1;
  static constexpr flag_t CAMERA_PITCH_LIMIT = 1 << 2;

  CameraConfig config;
  glm::vec3 position;
  SmoothVec3 target{8, 0};
  SmoothFloat angle{8, 0};
  SmoothFloat dist{8, 1};
  SmoothFloat pitch{8, 0};
  f32 vertOffset{0};
  flags_t flags = CAMERA_DIST_LIMIT | CAMERA_PITCH_LIMIT;

  TargetCamera() = default;

  TargetCamera(const TargetCamera& other, const CameraConfig config = CameraConfig()) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    config = rhs.config;
    position = rhs.position;
    target = rhs.target;
    angle = rhs.angle;
    dist = rhs.dist;
    pitch = rhs.pitch;
    vertOffset = rhs.vertOffset;
    flags = rhs.flags;
    return *this;
  }

  void aspect_ratio(f32 v) noexcept {
    config.aspect_ratio = v;
  }
};

inline f32 calculateTranslationFactor(const TargetCamera& cam) {
  auto result = cam.dist.current();
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= cam.config.spd_factor;
  result = std::max(cam.config.spd_factor_min, result);
  return result;
}

void updateCamera(TargetCamera&, const RunnerState&, const InputState&);

glm::mat4 getViewMatrix(const TargetCamera&);

glm::mat4 getPerspectiveMatrix(const TargetCamera&);

void getPerspectiveMatrix(glm::mat4* const ref, const TargetCamera&);

glm::mat4 getOrthographicMatrix(const TargetCamera&);

void getOrthographicMatrix(glm::mat4* const ref, const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_TARGET_CAMERA_HPP
