#ifndef UINTA_MATH_SMOOTH_FLOAT_HPP
#define UINTA_MATH_SMOOTH_FLOAT_HPP

#include <uinta/math/fwd.hpp>

namespace uinta {

class SmoothFloat {
 public:
  SmoothFloat(f32 agility, f32 start) noexcept : m_agility(agility), m_current(start), m_target(start) {
  }

  void force() {
    force(m_target);
  }

  void force(f32 value) {
    m_current = value;
    m_target = value;
  }

  inline void update(f32 dt) {
    m_current += (m_target - m_current) * m_agility * dt;
  }

  inline SmoothFloat& operator=(f32 v) noexcept {
    m_target = v;
    return *this;
  }

  inline SmoothFloat& operator+=(f32 v) noexcept {
    m_target += v;
    return *this;
  }

  inline SmoothFloat& operator-=(f32 v) noexcept {
    m_target -= v;
    return *this;
  }

  inline operator f32() const {
    return m_current;
  }

  f32 agility() const noexcept {
    return m_agility;
  }

  void agility(f32 v) noexcept {
    m_agility = v;
  }

  f32 current() const noexcept {
    return m_current;
  }

  void current(f32 v) noexcept {
    m_current = v;
  }

  f32 target() const noexcept {
    return m_target;
  }

  void target(f32 v) noexcept {
    m_target = v;
  }

 private:
  f32 m_agility;
  f32 m_current;
  f32 m_target;
};

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_FLOAT_HPP
