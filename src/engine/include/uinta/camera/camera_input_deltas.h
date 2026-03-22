#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_INPUT_DELTAS_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_INPUT_DELTAS_H_

#include <glm/ext/vector_float3.hpp>

#include "uinta/types.h"

namespace uinta {

struct CameraInputDeltas {
  f32 angle = 0;
  f32 pitch = 0;
  f32 dist = 0;
  glm::vec3 translation = glm::vec3(0);
  time_t translationDelta = 0;  // delta * translationFactor for smoothing
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_INPUT_DELTAS_H_
