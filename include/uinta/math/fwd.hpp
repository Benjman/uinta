#ifndef UINTA_MATH_FWD_HPP
#define UINTA_MATH_FWD_HPP

#include <uinta/types.h>

#include <glm/fwd.hpp>

namespace uinta {

struct SmoothFloat;
struct SmoothVec3;

inline glm::vec3 getForward(f32 pitch, f32 yaw);
inline f32 getPlaneInterceptDist(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection);
inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection);
inline glm::vec3 getPointOnRay(const glm::vec3& position, const glm::vec3& direction, f32 dist);
inline glm::vec3 getRight(f32 yaw);
inline glm::vec3 getUp(f32 pitch, f32 yaw);
inline glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& up);
inline glm::vec3 getWorldRay(const glm::vec2& cursorPos, const glm::vec2& viewportSize, const glm::mat4& view,
                             const glm::mat4& proj);

inline f32 map0to1Range(const f32 value, const f32 min, const f32 max);
inline f32 map0to1RangeClamped(const f32 value, const f32 min, const f32 max);
inline f32 clamp(f32 value, f32 min, f32 max);

inline void force(SmoothFloat&);
inline void force(SmoothFloat&, f32 value);
inline void update(SmoothFloat&, f32 dt);

inline void force(SmoothVec3&);
inline void force(SmoothVec3&, const glm::vec3& value);
inline void update(SmoothVec3&, f32 value);

}  // namespace uinta

#endif  // UINTA_MATH_FWD_HPP
