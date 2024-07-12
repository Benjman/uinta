#include "uinta/math/aabb.h"

#include "glm/common.hpp"
#include "uinta/types.h"

namespace uinta {

glm::vec3 to_vec3(const glm::vec3& point) noexcept { return point; }
glm::vec3 to_vec3(const glm::vec2& point) noexcept {
  return glm::vec3(point, 0.0);
}

template <typename T>
inline void init(AABB* a, const std::vector<T>& points) noexcept {
  static_assert(
      std::is_same<T, glm::vec3>::value || std::is_same<T, glm::vec2>::value,
      "T must be glm::vec2 or glm::vec3");
  assert(!points.empty() && "`points `cannot be null.");

  a->min = glm::vec3(std::numeric_limits<f32>::max());
  a->max = -glm::vec3(std::numeric_limits<f32>::max());

  for (const T& point : points) {
    auto p3d = to_vec3(point);
    a->min = glm::min(a->min, p3d);
    a->max = glm::max(a->max, p3d);
  }
}

AABB::AABB(const std::vector<glm::vec3>* points) noexcept {
  init(this, *points);
}

AABB::AABB(const std::vector<glm::vec2>* points) noexcept {
  init(this, *points);
}

}  // namespace uinta
