#ifndef UINTA_CAMERA_TARGET_CAMERA_HPP
#define UINTA_CAMERA_TARGET_CAMERA_HPP

#include <glm/fwd.hpp>
#include <uinta/camera/basic_camera.hpp>

namespace uinta {

class RunnerState;

struct TargetCamera : public BasicCamera {
  float dist;
  glm::vec3 target;

  TargetCamera(float dist) : BasicCamera(), dist(dist) {
  }

  TargetCamera(const TargetCamera& other) {
    *this = other;
  }

  TargetCamera& operator=(const TargetCamera& rhs) {
    BasicCamera::operator=(rhs);
    dist = rhs.dist;
    target = rhs.target;
    return *this;
  }
};

void update(TargetCamera&, const RunnerState&);
glm::mat4 getViewMatrix(const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_CAMERA_TARGET_CAMERA_HPP
