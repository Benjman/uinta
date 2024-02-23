#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_SPATIAL_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_SPATIAL_H_

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/math/defs.h"
#include "uinta/math/direction.h"
#include "uinta/types.h"

namespace uinta {

inline glm::vec3 getForward(f32 pitch, f32 yaw) noexcept;

inline f32 getPlaneInterceptDist(const glm::vec3& planePos,
                                 const glm::vec3& planeNormal,
                                 const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection,
                                 f32 minimum = epsilon_f) noexcept;

inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos,
                                        const glm::vec3& planeNormal,
                                        const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection) noexcept;

inline glm::vec3 getPointOnRay(const glm::vec3& position,
                               const glm::vec3& direction, f32 dist) noexcept;

inline glm::vec3 getRight(f32 yaw) noexcept;

inline glm::vec3 getUp(f32 pitch, f32 yaw) noexcept;

inline glm::vec3 getUp(const glm::vec3& forward,
                       const glm::vec3& right) noexcept;

inline glm::vec3 getWorldRay(const glm::vec2& cursorPos,
                             const glm::vec2& viewportSize,
                             const glm::mat4& view,
                             const glm::mat4& proj) noexcept;

inline glm::vec3 getWorldPoint(glm::vec2 viewport, glm::vec2 cursor,
                               glm::vec3 origin, glm::mat4 view, glm::mat4 proj,
                               glm::vec3 plane = glm::vec3(0)) noexcept;

inline glm::vec3 getForward(f32 pitch, f32 yaw) noexcept {
  auto pitchRad = glm::radians(pitch);
  auto yawRad = glm::radians(yaw);
  return {cos(pitchRad) * sin(yawRad), -sin(pitchRad),
          cos(pitchRad) * cos(yawRad)};
}

inline f32 getPlaneInterceptDist(const glm::vec3& planePos,
                                 const glm::vec3& planeNormal,
                                 const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection,
                                 f32 minimum) noexcept {
  auto denom = glm::dot(planeNormal, rayDirection);
  if (std::abs(denom) <= minimum) return 0;
  return glm::dot(planePos - rayPos, planeNormal) / denom;
}

inline glm::vec3 getPlaneInterceptPoint(
    const glm::vec3& planePos, const glm::vec3& planeNormal,
    const glm::vec3& rayPos, const glm::vec3& rayDirection) noexcept {
  auto dist =
      getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDirection);
  if (dist <= 0) return glm::vec3(INFINITY);
  return getPointOnRay(rayPos, rayDirection, dist);
}

inline glm::vec3 getPointOnRay(const glm::vec3& position,
                               const glm::vec3& direction, f32 dist) noexcept {
  return position + direction * dist;
}

inline glm::vec3 getRight(f32 yaw) noexcept {
  auto yawRad = glm::radians(yaw);
  return {cos(yawRad), 0, -sin(yawRad)};
}

inline glm::vec3 getUp(f32 pitch, f32 yaw) noexcept {
  return getUp(getForward(pitch, yaw), getRight(yaw));
}

inline glm::vec3 getUp(const glm::vec3& forward,
                       const glm::vec3& right) noexcept {
  return glm::cross(forward, right);
}

inline glm::vec3 getWorldRay(const glm::vec2& cursorPos,
                             const glm::vec2& viewport, const glm::mat4& view,
                             const glm::mat4& proj) noexcept {
  assert((viewport.y != 0 && viewport.x != 0) &&
         "Invalid viewport dimensions.");
  glm::vec3 ndc((2 * cursorPos.x) / viewport.x - 1,
                1 - (2 * cursorPos.y) / viewport.y, 1);
  glm::vec4 clip(ndc.x, ndc.y, -1, 1);
  glm::vec4 eye(glm::vec2(glm::inverse(proj) * clip), -1, 0);
  return glm::normalize(glm::inverse(view) * eye);
}

inline glm::vec3 getWorldPoint(glm::vec2 viewport, glm::vec2 cursor,
                               glm::vec3 origin, glm::mat4 view, glm::mat4 proj,
                               glm::vec3 plane) noexcept {
  auto worldRay = getWorldRay(cursor, viewport, view, proj);
  return getPlaneInterceptPoint(plane, WorldUp, origin, worldRay);
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_SPATIAL_H_
