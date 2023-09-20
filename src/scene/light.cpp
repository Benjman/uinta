#include <glm/geometric.hpp>
#include <uinta/scene/light.hpp>

namespace uinta {

Light::Light(const glm::vec3& direction, const glm::vec3& pos, const glm::vec3& color) noexcept
    : direction(glm::normalize(direction)), pos(pos), color(color) {
}

Light::Light(const Light& other) noexcept {
  *this = other;
}

Light& Light::operator=(const Light& other) noexcept {
  direction = glm::normalize(other.direction);
  pos = other.pos;
  color = other.color;
  return *this;
}

}  // namespace uinta
