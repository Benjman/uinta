#ifndef UINTA_CAMERA_CONFIG_HPP
#define UINTA_CAMERA_CONFIG_HPP

#include <uinta/input/keys.hpp>

namespace uinta {

const flag_t CAMERA_DIST_LIMIT = 1 << 0;
const flag_t CAMERA_PITCH_LIMIT = 1 << 1;

struct CameraConfig {
  flags_t flags = CAMERA_DIST_LIMIT | CAMERA_PITCH_LIMIT;

  f32 fov = 45;

  f32 nearPlane = 0.07;
  f32 farPlane = 200;

  f32 translateSpeedKeyboard = 4;
  f32 translateSpeedMouse = 2.0;
  f32 translationSpeedDistFactor = .45;  // Scales translation speed as a factor proportional to distance
  f32 translationSpeedDistFactorMin = .15;
  input_key_t translateBackward = KEY_D;
  input_key_t translateForward = KEY_E;
  input_key_t translateLeft = KEY_S;
  input_key_t translateMouse = MOUSE_BUTTON_LEFT;
  input_key_t translateRight = KEY_F;

  f32 pitchMax = 90;
  f32 pitchMin = 0.5;
  f32 pitchSpeedKeyboard = 90;
  f32 pitchSpeedMouse = 20;
  input_key_t pitchDown = KEY_A;
  input_key_t pitchMouse = MOUSE_BUTTON_RIGHT;
  input_key_t pitchUp = KEY_Q;

  f32 distMax = 30;
  f32 distMin = 0.2;
  f32 distSpeedKeyboard = 6;
  f32 distSpeedMouse = 60;
  input_key_t distDown = KEY_V;
  input_key_t distUp = KEY_C;

  f32 angleSpeedKeyboard = 90;
  f32 angleSpeedMouse = 20;
  input_key_t angleLeft = KEY_W;
  input_key_t angleMouse = MOUSE_BUTTON_RIGHT;
  input_key_t angleRight = KEY_R;

  CameraConfig() {
  }
};

}  // namespace uinta

#endif  // UINTA_CAMERA_CONFIG_HPP
