#ifndef UINTA_MATH_DIRECTION_HPP
#define UINTA_MATH_DIRECTION_HPP

#include <glm/vec3.hpp>

namespace uinta {

const glm::vec3 WORLD_UP{0, 1, 0};
const glm::vec3 WORLD_FORWARD{0, 0, -1};

glm::vec3 getForward(float pitch, float yaw);
glm::vec3 getRight(float yaw);
glm::vec3 getUp(float pitch, float yaw);
glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& up);

}  // namespace uinta

#endif  // UINTA_MATH_DIRECTION_HPP
