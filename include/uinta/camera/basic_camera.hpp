#ifndef UINTA_CAMERA_BASIC_CAMERA_HPP
#define UINTA_CAMERA_BASIC_CAMERA_HPP

#include <uinta/math/direction.hpp>

namespace uinta {

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
