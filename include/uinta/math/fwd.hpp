#ifndef UINTA_MATH_FWD_HPP
#define UINTA_MATH_FWD_HPP

#include <glm/fwd.hpp>

namespace uinta {

struct SmoothFloat;
struct SmoothVec3;

inline glm::vec3 getForward(float pitch, float yaw);
inline float getPlaneInterceptDist(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                   const glm::vec3& rayDirection);
inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos, const glm::vec3& planeNormal, const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection);
inline glm::vec3 getPointOnRay(const glm::vec3& position, const glm::vec3& direction, float dist);
inline glm::vec3 getRight(float yaw);
inline glm::vec3 getUp(float pitch, float yaw);
inline glm::vec3 getUp(const glm::vec3& forward, const glm::vec3& up);
inline glm::vec3 getWorldRay(const glm::vec2& cursorPos, const glm::vec2& viewportSize, const glm::mat4& view,
                             const glm::mat4& proj);

inline float map0to1Range(const float value, const float min, const float max);
inline float map0to1RangeClamped(const float value, const float min, const float max);
inline float clamp(float value, float min, float max);

inline void force(SmoothFloat&);
inline void force(SmoothFloat&, float value);
inline void update(SmoothFloat&, float dt);

inline void force(SmoothVec3&);
inline void force(SmoothVec3&, const glm::vec3& value);
inline void update(SmoothVec3&, float value);

}  // namespace uinta

#endif  // UINTA_MATH_FWD_HPP
