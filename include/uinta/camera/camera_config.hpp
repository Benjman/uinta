#ifndef UINTA_CAMERA_CONFIG_HPP
#define UINTA_CAMERA_CONFIG_HPP

#include <uinta/input/keys.hpp>

namespace uinta {

const auto CAMERA_DIST_LIMIT = 1 << 0;
const auto CAMERA_PITCH_LIMIT = 1 << 1;

struct CameraConfig {
  flags_t flags = CAMERA_DIST_LIMIT | CAMERA_PITCH_LIMIT;

  float fov = 45;

  float nearPlane = 0.07;
  float farPlane = 50;

  float translateSpeedKeyboard = 4;
  float translateSpeedMouse = 0.5;
  float translationSpeedDistFactor = .45;  // Scales translation speed as a factor proportional to distance
  float translationSpeedDistFactorMin = .15;
  input_key_t translateBackward = KEY_D;
  input_key_t translateForward = KEY_E;
  input_key_t translateLeft = KEY_S;
  input_key_t translateMouse = MOUSE_BUTTON_LEFT;
  input_key_t translateRight = KEY_F;

  float pitchMax = 90;
  float pitchMin = 0.5;
  float pitchSpeedKeyboard = 90;
  float pitchSpeedMouse = 10;
  input_key_t pitchDown = KEY_A;
  input_key_t pitchMouse = MOUSE_BUTTON_RIGHT;
  input_key_t pitchUp = KEY_Q;

  float distMax = 30;
  float distMin = 0.2;
  float distSpeedKeyboard = 6;
  float distSpeedMouse = 60;
  input_key_t distDown = KEY_V;
  input_key_t distUp = KEY_C;

  float angleSpeedKeyboard = 90;
  float angleSpeedMouse = 10;
  input_key_t angleLeft = KEY_W;
  input_key_t angleMouse = MOUSE_BUTTON_RIGHT;
  input_key_t angleRight = KEY_R;

  CameraConfig() {
  }
};

}  // namespace uinta

#endif  // UINTA_CAMERA_CONFIG_HPP
