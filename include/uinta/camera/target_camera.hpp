#ifndef UINTA_TARGET_CAMERA_HPP
#define UINTA_TARGET_CAMERA_HPP

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <uinta/camera/camera_config.hpp>
#include <uinta/fwd.hpp>
#include <uinta/math/smooth_float.hpp>
#include <uinta/math/smooth_vec3.hpp>

namespace uinta {

// disclaimer about euler angles and gimbal locking ...
struct TargetCamera {
  glm::vec3 position;
  SmoothVec3 target{8, 0};
  SmoothFloat angle{8, 0};
  SmoothFloat dist{8, 1};
  SmoothFloat pitch{8, 0};
  float vertOffset{0};

  CameraConfig config;

  TargetCamera() = default;

  TargetCamera(const TargetCamera& other, const CameraConfig config = CameraConfig()) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    position = rhs.position;
    target = rhs.target;
    angle = rhs.angle;
    dist = rhs.dist;
    pitch = rhs.pitch;
    vertOffset = rhs.vertOffset;
    return *this;
  }
};

void update(TargetCamera&, const RunnerState&, const InputState&);
glm::mat4 getViewMatrix(const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_TARGET_CAMERA_HPP
