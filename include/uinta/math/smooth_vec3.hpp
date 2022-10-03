#ifndef UINTA_MATH_SMOOTH_VEC3_HPP
#define UINTA_MATH_SMOOTH_VEC3_HPP

#include <glm/fwd.hpp>
#include <uinta/math/smooth_float.hpp>

namespace uinta {

struct SmoothVec3 {
  SmoothFloat x;
  SmoothFloat y;
  SmoothFloat z;

  SmoothVec3(float agility, float x = 0, float y = 0, float z = 0);
  SmoothVec3(float agility, const glm::vec3&);

  SmoothVec3& operator+=(const glm::vec3&);

  operator glm::vec3() const;
};

void update(SmoothVec3&, float);
void force(uinta::SmoothVec3&);
void force(uinta::SmoothVec3&, const glm::vec3&);

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_VEC3_HPP
