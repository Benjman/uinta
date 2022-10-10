#ifndef UINTA_CAMERA_TARGET_CAMERA_HPP
#define UINTA_CAMERA_TARGET_CAMERA_HPP

#include <glm/fwd.hpp>
#include <uinta/math/direction.hpp>

namespace uinta {

class RunnerState;

// ... insert standard disclaimer here about euler angles and gimbal locking ...
struct TargetCamera {
  glm::vec3 position;
  glm::vec3 target;
  float pitch = 0.0;
  float yaw = 0.0;
  float dist;

  TargetCamera(float dist) : position(WORLD_UP), target(0), pitch(90), yaw(0), dist(dist) {
  }

  TargetCamera(const TargetCamera& other) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    target = rhs.target;
    pitch = rhs.pitch;
    yaw = rhs.yaw;
    dist = rhs.dist;
    return *this;
  }
};

void update(TargetCamera&, const RunnerState&);
glm::mat4 getViewMatrix(const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_CAMERA_TARGET_CAMERA_HPP
