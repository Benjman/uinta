#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_

#include <span>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/types.h"

namespace uinta {

struct AABB {
  glm::vec3 min = {0, 0, 0};
  glm::vec3 max = {0, 0, 0};

  explicit AABB(const std::span<glm::vec3> points) noexcept;
  explicit AABB(const std::span<glm::vec2> points) noexcept;
  explicit AABB(const std::span<f32> points, size_t components) noexcept;

  glm::vec3 middle() const noexcept { return (min + max) / 2.0f; }

  glm::vec3 volume() const noexcept {
    return {max.x - min.x, max.y - min.y, max.z - min.z};
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_
