#ifndef UINTA_MATH_DIRECTION_HPP
#define UINTA_MATH_DIRECTION_HPP

#include <glm/fwd.hpp>

namespace uinta {

extern const glm::vec3 WORLD_FORWARD;
extern const glm::vec3 WORLD_HORIZONTAL;
extern const glm::vec3 WORLD_RIGHT;
extern const glm::vec3 WORLD_UP;

glm::vec3 getForward(float pitch, float yaw);
glm::vec3 getRight(float yaw);
glm::vec3 getUp(float pitch, float yaw);
glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& up);

}  // namespace uinta

#endif  // UINTA_MATH_DIRECTION_HPP
