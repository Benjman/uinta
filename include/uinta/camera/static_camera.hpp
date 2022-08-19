#ifndef UINTA_CAMERA_STATIC_CAMERA_HPP
#define UINTA_CAMERA_STATIC_CAMERA_HPP

#include <glm/vec3.hpp>

namespace uinta {

struct StaticCamera {
  glm::vec3 position;
  float pitch;
  float yaw;

  StaticCamera() : StaticCamera(glm::vec3(0.0), 0.0, 0.0) {}

  StaticCamera(glm::vec3 position, float pitch, float yaw) : position(glm::vec3(position)), pitch(pitch), yaw(yaw) {}

  StaticCamera& operator=(const StaticCamera& other) {
    position = glm::vec3(other.position);
    pitch = other.pitch;
    yaw = other.yaw;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_CAMERA_STATIC_CAMERA_HPP
