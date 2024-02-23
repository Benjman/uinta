#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_DIRECTION_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_DIRECTION_H_

#include "glm/ext/vector_float3.hpp"

namespace uinta {

constexpr glm::vec3 WorldForward = glm::vec3(0, 0, -1);
constexpr glm::vec3 WorldRight = glm::vec3(1, 0, 0);
constexpr glm::vec3 WorldUp = glm::vec3(0, 1, 0);
constexpr glm::vec3 WorldHorizontal = WorldRight + WorldForward;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_DIRECTION_H_
