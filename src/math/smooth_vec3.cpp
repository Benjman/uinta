#include <glm/vec3.hpp>
#include <uinta/math/smooth_vec3.hpp>

uinta::SmoothVec3::SmoothVec3(float agility, float x, float y, float z) : x(x, agility), y(y, agility), z(z, agility) {
}

uinta::SmoothVec3::SmoothVec3(float agility, const glm::vec3& v) : x(v.x, agility), y(v.y, agility), z(v.z, agility) {
}

void uinta::update(uinta::SmoothVec3& v, float dt) {
  update(v.x, dt);
  update(v.y, dt);
  update(v.z, dt);
}

void uinta::force(uinta::SmoothVec3& v) {
  force(v, {v.x.target, v.y.target, v.z.target});
}

void uinta::force(uinta::SmoothVec3& v, const glm::vec3& value) {
  force(v.x, value.x);
  force(v.y, value.y);
  force(v.z, value.z);
}

uinta::SmoothVec3& uinta::SmoothVec3::operator+=(const glm::vec3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

uinta::SmoothVec3::operator glm::vec3() const {
  return {x.current, y.current, z.current};
}
