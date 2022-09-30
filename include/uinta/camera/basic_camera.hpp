#ifndef UINTA_CAMERA_BASIC_CAMERA_HPP
#define UINTA_CAMERA_BASIC_CAMERA_HPP

#include <glm/vec3.hpp>

namespace uinta {

const glm::vec3 WORLD_UP{0.0, 1.0, 0.0};
const glm::vec3 WORLD_RIGHT{0.0, 0.0, 1.0};
const glm::vec3 WORLD_FORWARD{1.0, 0.0, 0.0};

struct BasicCamera {
  glm::vec3 position;
  float pitch = 0.0;
  float yaw = 0.0;

  BasicCamera() : BasicCamera(glm::vec3(0.0), 0.0, 0.0) {
  }

  BasicCamera(glm::vec3 position, float pitch, float yaw) : position(position), pitch(pitch), yaw(yaw) {
  }

  BasicCamera& operator=(const BasicCamera& other) {
    position = glm::vec3(other.position);
    pitch = other.pitch;
    yaw = other.yaw;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_CAMERA_BASIC_CAMERA_HPP
