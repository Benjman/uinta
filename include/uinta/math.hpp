#ifndef UINTA_MATH_H
#define UINTA_MATH_H

#include <glm/mat4x4.hpp>

#include "./math/map.hpp"
#include "./math/running_avg.hpp"
#include "./math/smooth_float.hpp"

namespace uinta {
void updateViewMatrix(glm::mat4& view, const glm::vec3& pos, const float pitch, const float yaw);
}

#endif  // UINTA_MATH_H
