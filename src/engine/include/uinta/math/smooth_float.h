#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_

#include <cmath>

#include "uinta/math/defs.h"

namespace uinta {

class SmoothFloat {
 public:
  using value_type = f32;

  explicit SmoothFloat(value_type agility, value_type start = 0) noexcept
      : agility_(agility), current_(start), target_(start) {}

  void force() noexcept { force(target_); }

  void force(value_type value) noexcept {
    current_ = value;
    target_ = value;
  }

  void update(time_t dt) noexcept {
    auto diff = target_ - current_;
    if (fabs(diff) > epsilon_f) current_ += diff * agility_ * dt;
  }

  SmoothFloat& operator=(value_type v) noexcept {
    target_ = v;
    return *this;
  }

  SmoothFloat& operator+=(value_type v) noexcept {
    target_ += v;
    return *this;
  }

  SmoothFloat& operator-=(value_type v) noexcept {
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

  operator value_type() const noexcept { return current_; }

  value_type agility() const noexcept { return agility_; }

  void agility(value_type v) noexcept { agility_ = v; }

  value_type current() const noexcept { return current_; }

  void current(value_type v) noexcept { current_ = v; }

  value_type target() const noexcept { return target_; }

  void target(value_type v) noexcept { target_ = v; }

 private:
  value_type agility_;
  value_type current_;
  value_type target_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_FLOAT_H_
