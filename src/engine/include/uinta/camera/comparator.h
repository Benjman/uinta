#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_COMPARATOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_COMPARATOR_H_

#include <cstdlib>

#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera.h"
#include "uinta/math/defs.h"
#include "uinta/types.h"

namespace uinta {

struct CameraComparator {
  glm::vec3 position;
  f32 angle;
  f32 dist;
  f32 pitch;

  bool operator==(const Camera* camera) noexcept {
    return position == camera->position() &&
           std::abs(angle - camera->angle()) <= epsilon_f &&
           std::abs(dist - camera->dist()) <= epsilon_f &&
           std::abs(pitch - camera->pitch()) <= epsilon_f;
  }

  bool operator!=(const Camera* camera) noexcept { return !(*this == camera); }

  CameraComparator& operator=(const Camera& camera) {
    position = camera.position();
    angle = camera.angle();
    dist = camera.dist();
    pitch = camera.pitch();
    return *this;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_COMPARATOR_H_
