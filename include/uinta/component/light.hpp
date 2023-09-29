#ifndef UINTA_SCENE_LIGHT_HPP
#define UINTA_SCENE_LIGHT_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace uinta {

struct Light {
  glm::vec3 direction;
  glm::vec3 pos;
  glm::vec3 color;

  Light(const glm::vec3& direction, const glm::vec3& pos, const glm::vec3& color) noexcept
      : direction(glm::normalize(direction)), pos(pos), color(color) {
  }

  Light& operator=(const Light& other) noexcept;
};

}  // namespace uinta

#endif  // UINTA_SCENE_LIGHT_HPP
