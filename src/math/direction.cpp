#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <uinta/math/direction.hpp>

namespace uinta {

const glm::vec3 WORLD_UP{0, 1, 0};
const glm::vec3 WORLD_HORIZONTAL{1, 0, -1};
const glm::vec3 WORLD_RIGHT{1, 0, 0};
const glm::vec3 WORLD_FORWARD{0, 0, -1};

glm::vec3 getForward(float pitch, float yaw) {
  auto p = glm::radians(pitch);
  auto y = glm::radians(yaw);
  return {cos(p) * sin(y), -sin(p), cos(p) * cos(y)};
}

glm::vec3 getRight(float yaw) {
  auto y = glm::radians(yaw);
  return {cos(y), 0, -sin(y)};
}

glm::vec3 getUp(float pitch, float yaw) {
  return getUp(getForward(pitch, yaw), getRight(yaw));
}

glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& right) {
  return glm::cross(forward, right);
}

}  // namespace uinta
