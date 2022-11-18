#ifndef UINTA_MATH_SMOOTH_FLOAT_HPP
#define UINTA_MATH_SMOOTH_FLOAT_HPP

#include <uinta/math/fwd.hpp>

namespace uinta {

struct SmoothFloat final {
  float agility;
  float current;
  float target;

  SmoothFloat(float agility, float start) noexcept : agility(agility), current(start), target(start) {
  }

  SmoothFloat(const SmoothFloat& other) noexcept {
    *this = other;
  }

  inline SmoothFloat& operator=(const SmoothFloat& other) noexcept {
    agility = other.agility;
    current = other.current;
    target = other.target;
    return *this;
  }

  inline SmoothFloat& operator=(float v) noexcept {
    target = v;
    return *this;
  }

  inline SmoothFloat& operator+=(float v) noexcept {
    target += v;
    return *this;
  }

  inline SmoothFloat& operator-=(float v) noexcept {
    target -= v;
    return *this;
  }

  inline operator float() const {
    return current;
  }
};

inline void force(SmoothFloat& v) {
  force(v, v.target);
}

inline void force(SmoothFloat& v, float value) {
  v.current = value;
  v.target = value;
}

inline void update(SmoothFloat& v, float dt) {
  v.current += (v.target - v.current) * v.agility * dt;
}

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_FLOAT_HPP
