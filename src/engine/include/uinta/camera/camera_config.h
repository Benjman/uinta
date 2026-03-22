#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_

#include "uinta/flags.h"
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

    [[nodiscard]] bool isAngleLock() const noexcept {
      return (flags_ & AngleLockMask) != 0;
    }

    void isAngleLock(bool v) noexcept {
      flags_ &= ~AngleLockMask;
      if (v) {
        flags_ |= AngleLockMask;
      }
    }

    [[nodiscard]] bool isDistLimit() const noexcept {
      return (flags_ & DistLimitMask) != 0;
    }

    void isDistLimit(bool v) noexcept {
      flags_ &= ~DistLimitMask;
      if (v) {
        flags_ |= DistLimitMask;
      }
    }

    [[nodiscard]] bool isDistLock() const noexcept {
      return (flags_ & DistLockMask) != 0;
    }

    void isDistLock(bool v) noexcept {
      flags_ &= ~DistLockMask;
      if (v) {
        flags_ |= DistLockMask;
      }
    }

    [[nodiscard]] bool isPitchLimit() const noexcept {
      return (flags_ & PitchLimitMask) != 0;
    }

    void isPitchLimit(bool v) noexcept {
      flags_ &= ~PitchLimitMask;
      if (v) {
        flags_ |= PitchLimitMask;
      }
    }

    [[nodiscard]] bool isPitchLock() const noexcept {
      return (flags_ & PitchLockMask) != 0;
    }

    void isPitchLock(bool v) noexcept {
      flags_ &= ~PitchLockMask;
      if (v) {
        flags_ |= PitchLockMask;
      }
    }

    [[nodiscard]] bool isKeyboardEnabled() const noexcept {
      return (flags_ & KeyboardEnabledMask) != 0;
    }

    void isKeyboardEnabled(bool v) noexcept {
      flags_ &= ~KeyboardEnabledMask;
      if (v) {
        flags_ |= KeyboardEnabledMask;
      }
    }

    [[nodiscard]] bool isMouseEnabled() const noexcept {
      return (flags_ & MouseEnabledMask) != 0;
    }

    void isMouseEnabled(bool v) noexcept {
      flags_ &= ~MouseEnabledMask;
      if (v) {
        flags_ |= MouseEnabledMask;
      }
    }

   private:
    value_type flags_;
  } flags;

  f32 dstMax = 0;         // Maximum distance when clamped
  f32 dstMin = 0;         // Minimum distance when clamped
  f32 pitchMax = 0;       // Maximum pitch when clamped
  f32 pitchMin = 0;       // Minimum pitch when clamped
  f32 spdFactorMin = 0;   // Minimum speed factor
  f32 trnslScaleFct = 0;  // Translation speed scales proportional to distance
  f32 vertOff = 0;        // Vertical offset

  SmoothFloat angle = SmoothFloat(0);
  SmoothFloat dist = SmoothFloat(0);
  SmoothFloat pitch = SmoothFloat(0);
  SmoothVec3 target = SmoothVec3(0);
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_CONFIG_H_
