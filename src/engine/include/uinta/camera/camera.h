#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <utility>

#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_input_deltas.h"
#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"
#include "uinta/types.h"

namespace uinta {

class Camera {
 public:
  explicit Camera(CameraConfig config) : config_(std::move(config)) {}

  ~Camera() noexcept = default;
  Camera(const Camera&) noexcept = delete;
  Camera& operator=(const Camera&) noexcept = delete;
  Camera(Camera&& other) noexcept = delete;
  Camera& operator=(Camera&& other) noexcept = delete;

  [[nodiscard]] const SmoothFloat& angle() const noexcept {
    return config_.angle;
  }

  void angle(f32 angle) noexcept { config_.angle = angle; }

  void angle(SmoothFloat angle) noexcept { config_.angle = angle; }

  void enableKeyboard(bool v = true) noexcept {
    config_.flags.isKeyboardEnabled(v);
  }

  void enableMouse(bool v = true) noexcept { config_.flags.isMouseEnabled(v); }

  void lockAngle(bool v = true) noexcept { config_.flags.isAngleLock(v); }

  void lockDist(bool v = true) noexcept { config_.flags.isDistLock(v); }

  void lockPitch(bool v = true) noexcept { config_.flags.isPitchLock(v); }

  [[nodiscard]] const CameraConfig& config() const noexcept { return config_; }

  CameraConfig& config() noexcept { return config_; }

  void config(const CameraConfig& config) noexcept { config_ = config; }

  [[nodiscard]] const SmoothFloat& dist() const noexcept {
    return config_.dist;
  }

  void dist(f32 dist) noexcept { config_.dist = dist; }

  void dist(SmoothFloat dist) noexcept { config_.dist = dist; }

  [[nodiscard]] CameraConfig::Flags flags() const noexcept {
    return config_.flags;
  }

  CameraConfig::Flags& flags() noexcept { return config_.flags; }

  void flags(CameraConfig::Flags flags) noexcept {
    config_.flags = std::move(flags);
  }

  [[nodiscard]] const SmoothFloat& pitch() const noexcept {
    return config_.pitch;
  }

  void pitch(f32 pitch) noexcept { config_.pitch = pitch; }

  void pitch(SmoothFloat pitch) noexcept { config_.pitch = pitch; }

  [[nodiscard]] glm::vec3 position() const noexcept { return position_; }

  [[nodiscard]] const SmoothVec3& target() const noexcept {
    return config_.target;
  }

  void target(const SmoothVec3& target) noexcept { config_.target = target; }

  void target(const glm::vec3& target) noexcept { config_.target = target; }

  [[nodiscard]] f32 translationFactor() const noexcept;

  void update(time_t, const CameraInputDeltas&) noexcept;

  void force() noexcept {
    config_.target.force();
    config_.angle.force();
    config_.dist.force();
    config_.pitch.force();
  }

  [[nodiscard]] glm::mat4 viewMatrix() const noexcept;

 private:
  CameraConfig config_;
  glm::vec3 position_ = glm::vec3(0);
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_H_
