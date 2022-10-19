#ifndef UINTA_MATH_SMOOTH_VEC3_HPP
#define UINTA_MATH_SMOOTH_VEC3_HPP

#include <glm/vec3.hpp>
#include <uinta/math/smooth_float.hpp>

namespace uinta {

struct SmoothVec3 {
  SmoothFloat x;
  SmoothFloat y;
  SmoothFloat z;

  SmoothVec3(float agility, float x = 0, float y = 0, float z = 0) : x({agility, x}), y({agility, y}), z({agility, z}) {
  }

  SmoothVec3(float agility, const glm::vec3& v) : x({agility, v.x}), y({agility, v.y}), z({agility, v.z}) {
  }

  inline SmoothVec3& operator+=(const glm::vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  inline SmoothVec3& operator-=(const glm::vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  inline SmoothVec3& operator=(const glm::vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  inline operator glm::vec3() const {
    return {x.current, y.current, z.current};
  }
};

void update(SmoothVec3&, float);
void force(uinta::SmoothVec3&);
void force(uinta::SmoothVec3&, const glm::vec3&);

inline void update(uinta::SmoothVec3& v, float dt) {
  update(v.x, dt);
  update(v.y, dt);
  update(v.z, dt);
}

inline void force(uinta::SmoothVec3& v) {
  force(v, {v.x.target, v.y.target, v.z.target});
}

inline void force(uinta::SmoothVec3& v, const glm::vec3& value) {
  force(v.x, value.x);
  force(v.y, value.y);
  force(v.z, value.z);
}

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_VEC3_HPP
