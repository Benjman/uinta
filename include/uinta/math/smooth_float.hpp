#ifndef UINTA_MATH_SMOOTH_FLOAT_HPP
#define UINTA_MATH_SMOOTH_FLOAT_HPP

#include <uinta/math/fwd.hpp>

namespace uinta {

struct SmoothFloat final {
  f32 agility;
  f32 current;
  f32 target;

  SmoothFloat(f32 agility, f32 start) noexcept : agility(agility), current(start), target(start) {
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

  inline SmoothFloat& operator=(f32 v) noexcept {
    target = v;
    return *this;
  }

  inline SmoothFloat& operator+=(f32 v) noexcept {
    target += v;
    return *this;
  }

  inline SmoothFloat& operator-=(f32 v) noexcept {
    target -= v;
    return *this;
  }

  inline operator f32() const {
    return current;
  }
};

inline void force(SmoothFloat& v) {
  force(v, v.target);
}

inline void force(SmoothFloat& v, f32 value) {
  v.current = value;
  v.target = value;
}

inline void update(SmoothFloat& v, f32 dt) {
  v.current += (v.target - v.current) * v.agility * dt;
}

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_FLOAT_HPP
