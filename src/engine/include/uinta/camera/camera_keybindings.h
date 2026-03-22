#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_KEYBINDINGS_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_KEYBINDINGS_H_

#include "uinta/input/input_token.h"
#include "uinta/types.h"

namespace uinta {

struct CameraKeybindings {
  // Keyboard keys
  Key anglNegK = Key::Unspecified;   // Negative angle
  Key anglPosK = Key::Unspecified;   // Positive angle
  Key dstDecK = Key::Unspecified;    // Decreasing distance
  Key dstIncK = Key::Unspecified;    // Increasing distance
  Key pitchNegK = Key::Unspecified;  // Negative pitch
  Key pitchPosK = Key::Unspecified;  // Positive pitch
  Key backwardK = Key::Unspecified;  // Translation backward
  Key forwardK = Key::Unspecified;   // Translation forward
  Key leftK = Key::Unspecified;      // Translation left
  Key rightK = Key::Unspecified;     // Translation right

  // Mouse buttons
  MouseBtn angleM = MouseBtn::Unspecified;  // Mouse input for angle
  MouseBtn pitchM = MouseBtn::Unspecified;  // Mouse input for pitch
  MouseBtn trnslM = MouseBtn::Unspecified;  // Mouse button for translate

  // Keyboard speed settings
  f32 anglSpdK = 0;   // Angle delta from keyboard input
  f32 dstSpdK = 0;    // Distance delta from keyboard input
  f32 pitchSpdK = 0;  // Pitch delta from keyboard input
  f32 trnslSpdK = 0;  // Translation speed for keyboard input

  // Mouse speed settings
  f32 anglSpdM = 0;   // Angle delta from mouse input
  f32 dstSpdM = 0;    // Distance delta from mouse input
  f32 pitchSpdM = 0;  // Pitch delta from mouse input
  f32 trnslSpdM = 0;  // Translation speed for mouse input
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_KEYBINDINGS_H_
