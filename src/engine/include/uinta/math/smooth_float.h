#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_

#include <cmath>

#include "uinta/math/defs.h"

namespace uinta {

class SmoothFloat {
 public:
  explicit SmoothFloat(f32 agility, f32 start = 0) noexcept
      : agility_(agility), current_(start), target_(start) {}

  void force() noexcept { force(target_); }

  void force(f32 value) noexcept {
    current_ = value;
    target_ = value;
  }

  void update(f32 dt) noexcept {
    auto diff = target_ - current_;
    if (fabs(diff) > epsilon_f) current_ += diff * agility_ * dt;
  }

  SmoothFloat& operator=(f32 v) noexcept {
    target_ = v;
    return *this;
  }

  SmoothFloat& operator+=(f32 v) noexcept {
    target_ += v;
    return *this;
  }

  SmoothFloat& operator-=(f32 v) noexcept {
    target_ -= v;
    return *this;
  }

  bool operator==(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) <= epsilon_f;
  }

  bool operator!=(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) != epsilon_f;
  }

  bool operator>(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) > epsilon_f;
  }

  bool operator<(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) < epsilon_f;
  }

  bool operator>=(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) >= epsilon_f;
  }

  bool operator<=(const SmoothFloat& other) const noexcept {
    return fabs(current_ - other.current_) <= epsilon_f;
  }

  operator f32() const noexcept { return current_; }

  f32 agility() const noexcept { return agility_; }

  void agility(f32 v) noexcept { agility_ = v; }

  f32 current() const noexcept { return current_; }

  void current(f32 v) noexcept { current_ = v; }

  f32 target() const noexcept { return target_; }

  void target(f32 v) noexcept { target_ = v; }

 private:
  f32 agility_;
  f32 current_;
  f32 target_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_
