#ifndef UINTA_SCENE_LIGHT_HPP
#define UINTA_SCENE_LIGHT_HPP

#include <glm/vec3.hpp>

namespace uinta {

struct Light {
  glm::vec3 direction;
  glm::vec3 pos;
  glm::vec3 color;

  Light(const glm::vec3& direction, const glm::vec3& pos = {0, 0, 0}, const glm::vec3& color = {1, 1, 1}) noexcept;

  Light(const Light& other) noexcept;

  Light& operator=(const Light& other) noexcept;
};

}  // namespace uinta

#endif  // UINTA_SCENE_LIGHT_HPP
