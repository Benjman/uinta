#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_

#include <cstdlib>

#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera.h"
#include "uinta/math/defs.h"

namespace uinta {

struct CameraComparator {
  glm::vec3 position;

  bool operator==(const Camera* camera) noexcept {
    return glm::length(position - camera->position()) <= epsilon_f;
  }

  bool operator!=(const Camera* camera) noexcept {
    return glm::length(position - camera->position()) > epsilon_f;
  }

  CameraComparator& operator=(const Camera& camera) {
    position = camera.position();
    return *this;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_
