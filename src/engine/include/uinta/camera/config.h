#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CONFIG_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CONFIG_H_

#include "uinta/input.h"
#include "uinta/types.h"

namespace uinta {

struct CameraConfig {
  f32 anglSpdK = 90;       // Angle delta from keyboard input
  f32 anglSpdM = 20;       // Angle delta from mouse input
  f32 dstMax = 10;         // Maximum distance when clamped
  f32 dstMin = 0.2;        // Minimum distance when clamped
  f32 dstSpdK = 15;        // Distance delta from keyboard input
  f32 dstSpdM = 30;        // Distance delta from mouse input
  f32 far = 200;           // Far clipping plane
  f32 fov = 45;            // Field of view
  f32 near = 0.01;         // Near clipping plane
  f32 pitchMax = 90;       // Maximum pitch when clamped
  f32 pitchMin = 0.5;      // Minimum pitch when clamped
  f32 pitchSpdK = 90;      // Pitch delta from keyboard input
  f32 pitchSpdM = 20;      // Pitch delta from mouse input
  f32 spdFactor = 0.3;     // Translation speed scales proportional to distance
  f32 spdFactorMin = .66;  // Minimum speed factor
  f32 trnslSpdK = 10;      // Translation speed for keyboard input
  f32 trnslSpdM = 2;       // Translation speed for mouse input
  f32 orthoSize = 60;      // Size for the orthographic matrix boundaries
  f32 vertOff = 0;         // Vertical offset

  InputKey anglNegK = KEY_R;   // Keyboard input for negative angle delta
  InputKey anglPosK = KEY_W;   // Keyboard input for positive angle delta
  InputKey dstDecK = KEY_C;    // Keyboard input for decreasing distance
  InputKey dstIncK = KEY_V;    // Keyboard input for increasing distance
  InputKey pitchNegK = KEY_A;  // Keyboard input for negative pitch delta
  InputKey pitchPosK = KEY_Q;  // Keyboard input for positive pitch delta
  InputKey backwardK = KEY_D;  // Keyboard input for translation backward
  InputKey forwardK = KEY_E;   // Keyboard input for translation forward
  InputKey leftK = KEY_S;      // Keyboard input for translation left
  InputKey rightK = KEY_F;     // Keyboard input for translation right

  MouseButton angleM = MOUSE_BUTTON_RIGHT;  // Mouse input for angle
  MouseButton pitchM = MOUSE_BUTTON_RIGHT;  // Mouse input for pitch
  MouseButton trnslM = MOUSE_BUTTON_LEFT;   // Mouse button for translate
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CONFIG_H_
