#ifndef UINTA_MATH_H
#define UINTA_MATH_H

#include <glm/mat4x4.hpp>
#include <uinta/math/map.hpp>
#include <uinta/math/running_avg.hpp>
#include <uinta/math/smooth_float.hpp>

namespace uinta {

void genViewMatrix(glm::mat4& view, const glm::vec3& pos, const float pitchDeg, const float yawDeg);

}  // namespace uinta

#endif  // UINTA_MATH_H
