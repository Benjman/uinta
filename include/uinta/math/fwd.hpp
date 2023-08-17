#ifndef UINTA_MATH_FWD_HPP
#define UINTA_MATH_FWD_HPP

#include <uinta/types.h>

#include <glm/fwd.hpp>

namespace uinta {

#define M_E 2.7182818284590452354         /* e */
#define M_LOG2E 1.4426950408889634074     /* log_2 e */
#define M_LOG10E 0.43429448190325182765   /* log_10 e */
#define M_LN2 0.69314718055994530942      /* log_e 2 */
#define M_LN10 2.30258509299404568402     /* log_e 10 */
#define M_PI 3.14159265358979323846       /* pi */
#define M_PI_2 1.57079632679489661923     /* pi/2 */
#define M_PI_4 0.78539816339744830962     /* pi/4 */
#define M_1_PI 0.31830988618379067154     /* 1/pi */
#define M_2_PI 0.63661977236758134308     /* 2/pi */
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#define M_SQRT2 1.41421356237309504880    /* sqrt(2) */
#define M_SQRT3 1.73205080756887729353    /* sqrt(3) */
#define M_SQRT1_2 0.70710678118654752440  /* 1/sqrt(2) */

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
