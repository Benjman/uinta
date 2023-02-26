#ifndef UINTA_COMPONENT_TRANFORM_HPP
#define UINTA_COMPONENT_TRANFORM_HPP

#include <glm/vec3.hpp>

namespace uinta {

struct Transform {
  glm::vec3 position{0};
  glm::vec3 scale{1};
  glm::vec3 rotation{0};
};

}  // namespace uinta

#endif  // UINTA_COMPONENT_TRANFORM_HPP
