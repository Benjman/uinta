#ifndef UINTA_CAMERA_CONFIG_HPP
#define UINTA_CAMERA_CONFIG_HPP

#include <uinta/input/keys.hpp>

namespace uinta {

struct CameraConfig {
  f32 angl_spd_k = 90;       // Speed of angle change from keyboard
  f32 angl_spd_m = 20;       // Speed of angle change from mouse
  f32 aspect_ratio = 1.0;    // Aspect ratio from window. Used to build projection matrices
  f32 dst_max = 30;          // Maximum value when distance is clamped
  f32 dst_min = 0.2;         // Minimum value when distance is clamped
  f32 dst_spd_k = 20;        // Speed of distance change from keyboard
  f32 dst_spd_m = 60;        // Speed of distance change from mouse
  f32 far = 200;             // Far clipping plane
  f32 fov = 45;              // Field of view
  f32 near = 0.07;           // Near clipping plane
  f32 pitch_max = 90;        // Maximum value when pitch is clamped
  f32 pitch_min = 0.5;       // Minimum value when pitch is clamped
  f32 pitch_spd_k = 90;      // Speed of pitch change from keyboard
  f32 pitch_spd_m = 20;      // Speed of pitch change from mouse
  f32 spd_factor = .45;      // Factor at which translation speed scales proportional to distance
  f32 spd_factor_min = .15;  // Minimum speed factor
  f32 trnsl_spd_k = 4;       // Translation speed for keyboard inputs
  f32 trnsl_spd_m = 2;       // Translation speed for mouse inputs

  input_key_t angl_lt_k = KEY_W;    // Keyboard input for angling to the left
  input_key_t angl_rt_k = KEY_R;    // Keyboard input for angling to the right
  input_key_t dst_dec_k = KEY_C;    // Keyboard input for decreasing distance
  input_key_t dst_inc_k = KEY_V;    // Keyboard input for increasing distance
  input_key_t pitch_dwn_k = KEY_A;  // Keyboard input for pitching down
  input_key_t pitch_up_k = KEY_Q;   // Keyboard input for pitching up
  input_key_t backward_k = KEY_D;   // Keyboard input for backward translation
  input_key_t forward_k = KEY_E;    // Keyboard input for forward translation
  input_key_t left_k = KEY_S;       // Keyboard input for left translation
  input_key_t right_k = KEY_F;      // Keyboard input for right translation

  mouse_button_t angke_m = MOUSE_BUTTON_RIGHT;  // Mouse input for angle
  mouse_button_t pitch_m = MOUSE_BUTTON_RIGHT;  // Mouse input for pitch
  mouse_button_t trnsl_m = MOUSE_BUTTON_LEFT;   // Mouse button for translation

  CameraConfig() {
  }

  CameraConfig(const CameraConfig& other) {
    *this = other;
  }

  CameraConfig& operator=(const CameraConfig& rhs) {
    angke_m = rhs.angke_m;
    angl_lt_k = rhs.angl_lt_k;
    angl_rt_k = rhs.angl_rt_k;
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
    pitch_dwn_k = rhs.pitch_dwn_k;
    pitch_m = rhs.pitch_m;
    pitch_max = rhs.pitch_max;
    pitch_min = rhs.pitch_min;
    pitch_spd_k = rhs.pitch_spd_k;
    pitch_spd_m = rhs.pitch_spd_m;
    pitch_up_k = rhs.pitch_up_k;
    right_k = rhs.right_k;
    spd_factor = rhs.spd_factor;
    spd_factor_min = rhs.spd_factor_min;
    trnsl_m = rhs.trnsl_m;
    trnsl_spd_k = rhs.trnsl_spd_k;
    trnsl_spd_m = rhs.trnsl_spd_m;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_CAMERA_CONFIG_HPP
