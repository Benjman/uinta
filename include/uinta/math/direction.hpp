#ifndef UINTA_MATH_UTILS_HPP
#define UINTA_MATH_UTILS_HPP

#include <uinta/types.h>

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace uinta {

constexpr glm::vec3 WORLD_FORWARD = glm::vec3(0, 0, -1);
constexpr glm::vec3 WORLD_RIGHT = glm::vec3(1, 0, 0);
constexpr glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);
constexpr glm::vec3 WORLD_HORIZONTAL = WORLD_RIGHT + WORLD_FORWARD;

inline glm::vec3 getForward(f32 pitch, f32 yaw);
inline f32 getPlaneInterceptDist(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection);
inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection);
inline glm::vec3 getPointOnRay(const glm::vec3& position, const glm::vec3& direction, f32 dist);
inline glm::vec3 getRight(f32 yaw);
inline glm::vec3 getUp(f32 pitch, f32 yaw);
inline glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& right);
inline glm::vec3 getWorldRay(const glm::vec2& cursorPos, const glm::vec2& viewportSize, const glm::mat4& view,
                             const glm::mat4& proj);

inline glm::vec3 getForward(f32 pitch, f32 yaw) {
  auto pitchRad = glm::radians(pitch);
  auto yawRad = glm::radians(yaw);
  return {cos(pitchRad) * sin(yawRad), -sin(pitchRad), cos(pitchRad) * cos(yawRad)};
}

inline f32 getPlaneInterceptDist(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
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

inline glm::vec3 getPointOnRay(const glm::vec3& position, const glm::vec3& direction, f32 dist) {
  return position + direction * dist;
}

inline glm::vec3 getRight(f32 yaw) {
  auto yawRad = glm::radians(yaw);
  return {cos(yawRad), 0, -sin(yawRad)};
}

inline glm::vec3 getUp(f32 pitch, f32 yaw) {
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
