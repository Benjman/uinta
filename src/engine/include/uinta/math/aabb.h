#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_

#include <limits>
#include <span>

#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/types.h"

namespace uinta {

struct AABB {
  glm::vec3 min = {0, 0, 0};
  glm::vec3 max = {0, 0, 0};

  AABB() noexcept = default;
  explicit AABB(const std::span<glm::vec3> points) noexcept;
  explicit AABB(const std::span<glm::vec2> points) noexcept;
  explicit AABB(const std::span<f32> points, size_t components) noexcept;

  static AABB empty() noexcept {
    AABB aabb;
    aabb.min = glm::vec3(std::numeric_limits<f32>::max());
    aabb.max = glm::vec3(std::numeric_limits<f32>::lowest());
    return aabb;
  }

  bool isValid() const noexcept {
    return min.x <= max.x && min.y <= max.y && min.z <= max.z;
  }

  glm::vec3 center() const noexcept { return (min + max) / 2.0f; }

  glm::vec3 size() const noexcept {
    return {max.x - min.x, max.y - min.y, max.z - min.z};
  }

  f32 volume() const noexcept {
    auto s = size();
    return s.x * s.y * s.z;
  }

  f32 surfaceArea() const noexcept {
    auto s = size();
    return 2.0f * (s.x * s.y + s.y * s.z + s.z * s.x);
  }

  bool contains(const glm::vec3& point) const noexcept {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y &&
           point.y <= max.y && point.z >= min.z && point.z <= max.z;
  }

  bool intersects(const AABB& other) const noexcept {
    return min.x <= other.max.x && max.x >= other.min.x &&
           min.y <= other.max.y && max.y >= other.min.y &&
           min.z <= other.max.z && max.z >= other.min.z;
  }

  void expand(const glm::vec3& point) noexcept {
    min = glm::min(min, point);
    max = glm::max(max, point);
  }

  void expand(const AABB& other) noexcept {
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_AABB_H_