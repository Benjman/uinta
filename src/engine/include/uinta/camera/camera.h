#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_

#include "glm/ext/matrix_float4x4.hpp"
#include "uinta/camera/camera_config.h"
#include "uinta/flags.h"
#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;

class Camera {
 public:
  struct Flags final {
    using value_type = u8;

    static constexpr value_type DistLimitMask = 1 << 0;
    static constexpr value_type PitchLimitMask = 1 << 1;

    FlagsOperations(DistLimitMask | PitchLimitMask);

    bool isDistLimit() const noexcept { return flags_ & DistLimitMask; }
    void isDistLimit(bool v) noexcept {
      flags_ &= ~DistLimitMask;
      if (v) flags_ |= DistLimitMask;
    }

    bool isPitchLimit() const noexcept { return flags_ & PitchLimitMask; }
    void isPitchLimit(bool v) noexcept {
      flags_ &= ~PitchLimitMask;
      if (v) flags_ |= PitchLimitMask;
    }

   private:
    value_type flags_;
  };

  explicit Camera(CameraConfig config = {}) noexcept : config_(config) {}

  ~Camera() noexcept = default;
  Camera(const Camera&) noexcept = delete;
  Camera& operator=(const Camera&) noexcept = delete;
  Camera(Camera&& other) noexcept = delete;
  Camera& operator=(Camera&& other) noexcept = delete;

  const SmoothFloat& angle() const noexcept { return angle_; }

  void angle(f32 angle) noexcept { angle_ = angle; }

  void angle(SmoothFloat angle) noexcept { angle_ = angle; }

  const CameraConfig& config() const noexcept { return config_; }

  void config(const CameraConfig& config) noexcept { config_ = config; }

  const SmoothFloat& dist() const noexcept { return dist_; }

  void dist(f32 dist) noexcept { dist_ = dist; }

  void dist(SmoothFloat dist) noexcept { dist_ = dist; }

  Flags flags() const noexcept { return flags_; }

  void flags(Flags flags) noexcept { flags_ = flags; }

  glm::mat4 perspectiveMatrix(f32 aspectRatio) const noexcept;

  glm::mat4 orthographicMatrix() const noexcept;

  const SmoothFloat& pitch() const noexcept { return pitch_; }

  void pitch(f32 pitch) noexcept { pitch_ = pitch; }

  void pitch(SmoothFloat pitch) noexcept { pitch_ = pitch; }

  glm::vec3 position() const noexcept { return position_; }

  const SmoothVec3& target() const noexcept { return target_; }

  void target(const SmoothVec3& target) noexcept { target_ = target; }

  void target(const glm::vec3& target) noexcept { target_ = target; }

  f32 translationFactor() const noexcept;

  void update(const EngineState&) noexcept;

  void force() noexcept {
    target_.force();
    angle_.force();
    dist_.force();
    pitch_.force();
  }

  glm::mat4 viewMatrix() const noexcept;

 private:
  CameraConfig config_ = {};
  glm::vec3 position_ = {};
  SmoothVec3 target_ = SmoothVec3(8);
  SmoothFloat angle_ = SmoothFloat(8);
  SmoothFloat dist_ = SmoothFloat(8);
  SmoothFloat pitch_ = SmoothFloat(8);
  Flags flags_;

  void updateAngle(time_t, const Input&) noexcept;
  void updateDist(time_t, const Input&) noexcept;
  void updatePitch(time_t, const Input&) noexcept;
  void updateTranslation(time_t, const Input&) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
