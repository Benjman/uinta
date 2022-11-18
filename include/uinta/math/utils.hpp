#ifndef UINTA_MATH_UTILS_HPP
#define UINTA_MATH_UTILS_HPP

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <uinta/math/fwd.hpp>

namespace uinta {

inline glm::vec3 getForward(float pitch, float yaw) {
  auto pitchRad = glm::radians(pitch);
  auto yawRad = glm::radians(yaw);
  return {cos(pitchRad) * sin(yawRad), -sin(pitchRad), cos(pitchRad) * cos(yawRad)};
}

inline float getPlaneInterceptDist(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                   const glm::vec3& rayDirection) {
  auto denom = glm::dot(planeNormal, rayDirection);
  if (std::abs(denom) <= 1e-6) return 0;
  return glm::dot(planePos - rayPos, planeNormal) / denom;
}

inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection) {
  auto dist = getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDirection);
  if (dist <= 0) return glm::vec3(INFINITY);
  return getPointOnRay(rayPos, rayDirection, dist);
}

inline glm::vec3 getPointOnRay(const glm::vec3& position, const glm::vec3& direction, float dist) {
  return position + direction * dist;
}

inline glm::vec3 getRight(float yaw) {
  auto yawRad = glm::radians(yaw);
  return {cos(yawRad), 0, -sin(yawRad)};
}

inline glm::vec3 getUp(float pitch, float yaw) {
  return getUp(getForward(pitch, yaw), getRight(yaw));
}

inline glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& right) {
  return glm::cross(forward, right);
}

inline glm::vec3 getWorldRay(const glm::vec2& cursorPos, const glm::vec2& viewportSize, const glm::mat4& view,
                             const glm::mat4& proj) {
  glm::vec3 ndc = {(2 * cursorPos.x) / viewportSize.x - 1, 1 - (2 * cursorPos.y) / viewportSize.y, 1};
  glm::vec4 clip = {ndc.x, ndc.y, -1, 1};
  glm::vec4 eye = {glm::vec2(glm::inverse(proj) * clip), -1, 0};
  return glm::normalize(glm::inverse(view) * eye);
}

}  // namespace uinta

#endif  // UINTA_MATH_UTILS_HPP
