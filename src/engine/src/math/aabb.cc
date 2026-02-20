#include "uinta/math/aabb.h"

#include "uinta/types.h"

namespace uinta {

glm::vec3 to_vec3(const glm::vec3& point) noexcept { return point; }
glm::vec3 to_vec3(const glm::vec2& point) noexcept {
  return glm::vec3(point, 0.0);
}

template <typename T>
inline void init(AABB* a, const std::span<T> points) noexcept {
  static_assert(std::is_same_v<T, glm::vec3> || std::is_same_v<T, glm::vec2>,
                "Must be glm::vec2 or glm::vec3");
  assert(!points.empty() && "`points `cannot be null.");

  a->min = glm::vec3(std::numeric_limits<f32>::max());
  a->max = glm::vec3(std::numeric_limits<f32>::lowest());

  for (const T& point : points) {
    auto p3d = to_vec3(point);
    a->min = glm::min(a->min, p3d);
    a->max = glm::max(a->max, p3d);
  }
}

AABB::AABB(const std::span<glm::vec3> points) noexcept { init(this, points); }

AABB::AABB(const std::span<glm::vec2> points) noexcept { init(this, points); }

AABB::AABB(const std::span<f32> data, size_t components) noexcept {
  assert((components == 2 || components == 3) && "Component count invalid");
  assert(!data.empty() && "data cannot be empty");

  min = glm::vec3(std::numeric_limits<f32>::max());
  max = glm::vec3(std::numeric_limits<f32>::lowest());

  for (size_t i = 0; i < data.size(); i += components) {
    glm::vec3 p = (components == 2)
                      ? glm::vec3(data[i], data[i + 1], 0.0f)
                      : glm::vec3(data[i], data[i + 1], data[i + 2]);
    min = glm::min(min, p);
    max = glm::max(max, p);
  }
}

}  // namespace uinta