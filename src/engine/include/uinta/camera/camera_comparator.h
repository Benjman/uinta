#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_

#include <cstdlib>

#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera.h"
#include "uinta/math/defs.h"

namespace uinta {

struct CameraComparator {
  bool operator==(const Camera* camera) const noexcept {
    return glm::length(position - camera->position()) <= epsilon_f &&
           abs(pitch - camera->pitch().current()) <= epsilon_f &&
           abs(angle - camera->angle().current()) <= epsilon_f;
  }

  bool operator!=(const Camera* camera) const noexcept {
    return glm::length(position - camera->position()) > epsilon_f ||
           abs(pitch - camera->pitch().current()) > epsilon_f ||
           abs(angle - camera->angle().current()) > epsilon_f;
  }

  CameraComparator& operator=(const Camera& camera) {
    position = camera.position();
    pitch = camera.pitch();
    angle = camera.angle();
    return *this;
  }

 private:
  glm::vec3 position;
  f32 pitch;
  f32 angle;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_COMPARATOR_H_
