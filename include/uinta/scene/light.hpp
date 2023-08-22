#ifndef UINTA_SCENE_LIGHT_HPP
#define UINTA_SCENE_LIGHT_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace uinta {

struct Light {
  glm::vec3 direction;
  glm::vec3 pos;
  glm::vec3 color;

  Light(const glm::vec3& direction, const glm::vec3& pos = {0, 0, 0}, const glm::vec3& color = {1, 1, 1}) noexcept
      : direction(glm::normalize(direction)), pos(pos), color(color) {
  }

  Light(const Light& other) noexcept {
    *this = other;
  }

  Light& operator=(const Light& other) noexcept {
    direction = glm::normalize(other.direction);
    pos = other.pos;
    color = other.color;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_SCENE_LIGHT_HPP
