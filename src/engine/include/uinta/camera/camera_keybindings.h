#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_KEYBINDINGS_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_KEYBINDINGS_H_

#include "uinta/input/fwd.h"
#include "uinta/types.h"

namespace uinta {

struct CameraKeybindings {
  // Keyboard keys
  InputKey anglNegK = _::InputUnknown;   // Negative angle
  InputKey anglPosK = _::InputUnknown;   // Positive angle
  InputKey dstDecK = _::InputUnknown;    // Decreasing distance
  InputKey dstIncK = _::InputUnknown;    // Increasing distance
  InputKey pitchNegK = _::InputUnknown;  // Negative pitch
  InputKey pitchPosK = _::InputUnknown;  // Positive pitch
  InputKey backwardK = _::InputUnknown;  // Translation backward
  InputKey forwardK = _::InputUnknown;   // Translation forward
  InputKey leftK = _::InputUnknown;      // Translation left
  InputKey rightK = _::InputUnknown;     // Translation right

  // Mouse buttons
  MouseButton angleM = _::InputUnknown;  // Mouse input for angle
  MouseButton pitchM = _::InputUnknown;  // Mouse input for pitch
  MouseButton trnslM = _::InputUnknown;  // Mouse button for translate

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
