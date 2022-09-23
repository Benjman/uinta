#ifndef UINTA_CAMERA_BASIC_CAMERA_HPP
#define UINTA_CAMERA_BASIC_CAMERA_HPP

#include <glm/vec3.hpp>

namespace uinta {

inline const glm::vec3 WORLD_UP = glm::vec3(0.0, 0.0, 1.0);
inline const glm::vec3 WORLD_RIGHT = glm::vec3(1.0, 0.0, 0.0);
inline const glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 0.0, -1.0);

struct BasicCamera {
  glm::vec3 position;
  float pitch = 0.0;
  float yaw = 0.0;

  BasicCamera();
  BasicCamera(glm::vec3 position, float pitch, float yaw);
  BasicCamera& operator=(const BasicCamera& other);
};

}  // namespace uinta

#endif  // UINTA_CAMERA_BASIC_CAMERA_HPP
