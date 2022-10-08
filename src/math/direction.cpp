#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <uinta/math/direction.hpp>

glm::vec3 uinta::getForward(float pitch, float yaw) {
  auto p = glm::radians(pitch);
  auto y = glm::radians(yaw);
  return {cos(p) * sin(y), -sin(p), cos(p) * cos(y)};
}

glm::vec3 uinta::getRight(float yaw) {
  auto y = glm::radians(yaw);
  return {cos(y), 0, -sin(y)};
}

glm::vec3 uinta::getUp(float pitch, float yaw) {
  return getUp(getForward(pitch, yaw), getRight(yaw));
}

glm::vec3 uinta::getUp(const glm::vec3& forward, const glm::vec3& right) {
  return glm::cross(forward, right);
}
