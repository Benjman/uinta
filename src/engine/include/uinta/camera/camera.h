#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_

#include "glm/ext/matrix_float4x4.hpp"
#include "uinta/camera/config.h"
#include "uinta/input.h"
#include "uinta/math/smooth_vec.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;

class Camera {
  using CameraFlags = u32;

 public:
  struct Masks final {
    static constexpr CameraFlags DistLimit = 1 << 0;
    static constexpr CameraFlags PitchLimit = 1 << 1;
  };

  explicit Camera(CameraConfig config = {}) noexcept : config_(config) {}

  ~Camera() noexcept = default;
  Camera(const Camera&) noexcept = delete;
  Camera& operator=(const Camera&) noexcept = delete;
  Camera(Camera&& other) noexcept = delete;
  Camera& operator=(Camera&& other) noexcept = delete;

  const SmoothFloat& angle() const noexcept { return angle_; }

  void angle(f32 angle) noexcept { angle_ = angle; }

  const CameraConfig& config() const noexcept { return config_; }

  void config(const CameraConfig& config) noexcept { config_ = config; }

  const SmoothFloat& dist() const noexcept { return dist_; }

  void dist(f32 dist) noexcept { dist_ = dist; }

  CameraFlags& flags() noexcept { return flags_; }

  glm::mat4 perspectiveMatrix(f32 aspectRatio) const noexcept;

  glm::mat4 orthographicMatrix() const noexcept;

  const SmoothFloat& pitch() const noexcept { return pitch_; }

  void pitch(f32 pitch) noexcept { pitch_ = pitch; }

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

  bool limitDist() noexcept { return flags_ & Masks::DistLimit; }

  void limitDist(bool v) noexcept {
    flags_ &= ~Masks::DistLimit;
    if (v) flags_ |= Masks::DistLimit;
  }

  bool limitPitch() noexcept { return flags_ & Masks::PitchLimit; }

  void limitPitch(bool v) noexcept {
    flags_ &= ~Masks::PitchLimit;
    if (v) flags_ |= Masks::PitchLimit;
  }

 private:
  CameraConfig config_ = {};
  glm::vec3 position_ = {};
  SmoothVec3 target_ = SmoothVec3(8);
  SmoothFloat angle_ = SmoothFloat(8);
  SmoothFloat dist_ = SmoothFloat(8);
  SmoothFloat pitch_ = SmoothFloat(8);
  CameraFlags flags_ = Masks::DistLimit | Masks::PitchLimit;

  void processInput(const EngineState&) noexcept;
  void updatePosition(const EngineState&) noexcept;
  void updateAngle(f32, const Input&) noexcept;
  void updateDist(f32, const Input&) noexcept;
  void updatePitch(f32, const Input&) noexcept;
  void updateTranslation(f32, const Input&) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
