#ifndef UINTA_TARGET_CAMERA_HPP
#define UINTA_TARGET_CAMERA_HPP

#include <glm/vec3.hpp>
#include <uinta/camera/camera_config.hpp>
#include <uinta/camera/fwd.hpp>
#include <uinta/math/smooth_float.hpp>
#include <uinta/math/smooth_vec3.hpp>

namespace uinta {

// disclaimer about euler angles and gimbal locking ...
struct TargetCamera {
  static constexpr flag_t CAMERA_DIST_LIMIT = 1 << 1;
  static constexpr flag_t CAMERA_PITCH_LIMIT = 1 << 2;

  CameraConfig config;
  glm::vec3 position;
  SmoothVec3 target{8, 0};
  SmoothFloat angle{8, 0};
  SmoothFloat dist{8, 1};
  SmoothFloat pitch{8, 0};
  f32 vertOffset{0};
  flags_t flags = CAMERA_DIST_LIMIT | CAMERA_PITCH_LIMIT;

  TargetCamera() = default;

  TargetCamera(const TargetCamera& other, const CameraConfig config = CameraConfig()) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    config = rhs.config;
    position = rhs.position;
    target = rhs.target;
    angle = rhs.angle;
    dist = rhs.dist;
    pitch = rhs.pitch;
    vertOffset = rhs.vertOffset;
    flags = rhs.flags;
    return *this;
  }

  void aspect_ratio(f32 v) noexcept {
    config.aspect_ratio = v;
  }
};

inline f32 calculateTranslationFactor(const TargetCamera& cam) {
  auto result = cam.dist.current;
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= cam.config.spd_factor;
  result = std::max(cam.config.spd_factor_min, result);
  return result;
}

}  // namespace uinta

#endif  // UINTA_TARGET_CAMERA_HPP
