#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_

#include <string>

#include "uinta/flags.h"
#include "uinta/input/fwd.h"
#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"
#include "uinta/types.h"

namespace uinta {

struct CameraConfig {
  struct Flags final {
    using value_type = u8;

    static constexpr value_type DistLimitMask = 1 << 0;
    static constexpr value_type PitchLimitMask = 1 << 1;
    static constexpr value_type KeyboardEnabledMask = 1 << 2;
    static constexpr value_type MouseEnabledMask = 1 << 3;
    static constexpr value_type AngleLockMask = 1 << 4;
    static constexpr value_type DistLockMask = 1 << 5;
    static constexpr value_type PitchLockMask = 1 << 6;

    static constexpr value_type DefaultFlags =
        DistLimitMask | PitchLimitMask | KeyboardEnabledMask | MouseEnabledMask;

    FlagsOperations(DefaultFlags);

    bool isAngleLock() const noexcept { return flags_ & AngleLockMask; }
    void isAngleLock(bool v) noexcept {
      flags_ &= ~AngleLockMask;
      if (v) flags_ |= AngleLockMask;
    }

    bool isDistLimit() const noexcept { return flags_ & DistLimitMask; }
    void isDistLimit(bool v) noexcept {
      flags_ &= ~DistLimitMask;
      if (v) flags_ |= DistLimitMask;
    }

    bool isDistLock() const noexcept { return flags_ & DistLockMask; }
    void isDistLock(bool v) noexcept {
      flags_ &= ~DistLockMask;
      if (v) flags_ |= DistLockMask;
    }

    bool isPitchLimit() const noexcept { return flags_ & PitchLimitMask; }
    void isPitchLimit(bool v) noexcept {
      flags_ &= ~PitchLimitMask;
      if (v) flags_ |= PitchLimitMask;
    }

    bool isPitchLock() const noexcept { return flags_ & PitchLockMask; }
    void isPitchLock(bool v) noexcept {
      flags_ &= ~PitchLockMask;
      if (v) flags_ |= PitchLockMask;
    }

    bool isKeyboardEnabled() const noexcept {
      return flags_ & KeyboardEnabledMask;
    }
    void isKeyboardEnabled(bool v) noexcept {
      flags_ &= ~KeyboardEnabledMask;
      if (v) flags_ |= KeyboardEnabledMask;
    }

    bool isMouseEnabled() const noexcept { return flags_ & MouseEnabledMask; }
    void isMouseEnabled(bool v) noexcept {
      flags_ &= ~MouseEnabledMask;
      if (v) flags_ |= MouseEnabledMask;
    }

   private:
    value_type flags_;
  } flags;

  enum class Projection : u8 {
    Perspective,
    Orthographic,
    Unknown,
  } projection = Projection::Unknown;

  f32 anglSpdK = 0;       // Angle delta from keyboard input
  f32 anglSpdM = 0;       // Angle delta from mouse input
  f32 dstMax = 0;         // Maximum distance when clamped
  f32 dstMin = 0;         // Minimum distance when clamped
  f32 dstSpdK = 0;        // Distance delta from keyboard input
  f32 dstSpdM = 0;        // Distance delta from mouse input
  f32 far = 0;            // Far clipping plane
  f32 fov = 0;            // Field of view
  f32 near = 0;           // Near clipping plane
  f32 orthoSize = 0;      // Orthographic size
  f32 pitchMax = 0;       // Maximum pitch when clamped
  f32 pitchMin = 0;       // Minimum pitch when clamped
  f32 pitchSpdK = 0;      // Pitch delta from keyboard input
  f32 pitchSpdM = 0;      // Pitch delta from mouse input
  f32 spdFactorMin = 0;   // Minimum speed factor
  f32 trnslScaleFct = 0;  // Translation speed scales proportional to distance
  f32 trnslSpdK = 0;      // Translation speed for keyboard input
  f32 trnslSpdM = 0;      // Translation speed for mouse input
  f32 vertOff = 0;        // Vertical offset

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

  MouseButton angleM = _::InputUnknown;  // Mouse input for angle
  MouseButton pitchM = _::InputUnknown;  // Mouse input for pitch
  MouseButton trnslM = _::InputUnknown;  // Mouse button for translate

  SmoothFloat angle = SmoothFloat(0);
  SmoothFloat dist = SmoothFloat(0);
  SmoothFloat pitch = SmoothFloat(0);
  SmoothVec3 target = SmoothVec3(0);
};

using CameraProjection = CameraConfig::Projection;

inline std::string toString(CameraProjection projection) noexcept {
  switch (projection) {
    case (CameraProjection::Orthographic):
      return "Orthographic";
    case (CameraProjection::Perspective):
      return "Perspective";
    default:
      return "Unknown";
  }
}

inline CameraProjection toProjection(std::string projection) noexcept {
  if ("Perspective" == projection) {
    return CameraProjection::Perspective;
  } else if ("Orthographic" == projection) {
    return CameraProjection::Orthographic;
  } else {
    return CameraProjection::Unknown;
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_
