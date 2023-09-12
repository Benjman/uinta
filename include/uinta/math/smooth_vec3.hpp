#ifndef UINTA_MATH_SMOOTH_VEC3_HPP
#define UINTA_MATH_SMOOTH_VEC3_HPP

#include <glm/vec3.hpp>
#include <uinta/math/smooth_float.hpp>

namespace uinta {

class SmoothVec3 {
 public:
  SmoothVec3(f32 agility, f32 x = 0, f32 y = 0, f32 z = 0) : m_x({agility, x}), m_y({agility, y}), m_z({agility, z}) {
  }

  SmoothVec3(f32 agility, const glm::vec3& v) : m_x({agility, v.x}), m_y({agility, v.y}), m_z({agility, v.z}) {
  }

  void agility(f32 agility) {
    m_x.agility(agility);
    m_y.agility(agility);
    m_z.agility(agility);
  }

  void force() {
    force({m_x.target(), m_y.target(), m_z.target()});
  }

  void force(const glm::vec3& value) {
    m_x.force(value.x);
    m_y.force(value.y);
    m_z.force(value.z);
  }

  void update(f32 dt) {
    m_x.update(dt);
    m_y.update(dt);
    m_z.update(dt);
  }

  SmoothVec3& operator+=(const glm::vec3& v) noexcept {
    m_x += v.x;
    m_y += v.y;
    m_z += v.z;
    return *this;
  }

  SmoothVec3& operator-=(const glm::vec3& v) noexcept {
    m_x -= v.x;
    m_y -= v.y;
    m_z -= v.z;
    return *this;
  }

  SmoothVec3& operator=(const glm::vec3& v) noexcept {
    m_x = v.x;
    m_y = v.y;
    m_z = v.z;
    return *this;
  }

  operator glm::vec3() const noexcept {
    return {m_x, m_y, m_z};
  }

  SmoothFloat x() const noexcept {
    return m_x;
  }

  void x(const SmoothFloat& v) noexcept {
    m_x = v;
  }

  SmoothFloat y() const noexcept {
    return m_y;
  }

  void y(const SmoothFloat& v) noexcept {
    m_y = v;
  }

  SmoothFloat z() const noexcept {
    return m_z;
  }

  void z(const SmoothFloat& v) noexcept {
    m_z = v;
  }

 private:
  SmoothFloat m_x;
  SmoothFloat m_y;
  SmoothFloat m_z;
};

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_VEC3_HPP
