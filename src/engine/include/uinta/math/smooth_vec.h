#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_VEC_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_VEC_H_

#include "glm/ext/vector_float3.hpp"
#include "uinta/math/smooth_float.h"

namespace uinta {

class SmoothVec3 {
 public:
  explicit SmoothVec3(f32 agility, f32 x = 0, f32 y = 0, f32 z = 0) noexcept
      : x_(agility, x), y_(agility, y), z_(agility, z) {}

  SmoothVec3(f32 agility, const glm::vec3& v) noexcept
      : x_(agility, v.x), y_(agility, v.y), z_(agility, v.z) {}

  void agility(f32 agility) noexcept {
    x_.agility(agility);
    y_.agility(agility);
    z_.agility(agility);
  }

  void force() noexcept { force({x_.target(), y_.target(), z_.target()}); }

  void force(const glm::vec3& value) noexcept {
    x_.force(value.x);
    y_.force(value.y);
    z_.force(value.z);
  }

  void update(f32 dt) noexcept {
    x_.update(dt);
    y_.update(dt);
    z_.update(dt);
  }

  SmoothVec3& operator+=(const glm::vec3& v) noexcept {
    x_ += v.x;
    y_ += v.y;
    z_ += v.z;
    return *this;
  }

  SmoothVec3& operator-=(const glm::vec3& v) noexcept {
    x_ -= v.x;
    y_ -= v.y;
    z_ -= v.z;
    return *this;
  }

  SmoothVec3& operator=(const glm::vec3& v) noexcept {
    x_ = v.x;
    y_ = v.y;
    z_ = v.z;
    return *this;
  }

  bool operator==(const SmoothVec3& other) const noexcept {
    return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
  }

  bool operator!=(const SmoothVec3& other) const noexcept {
    return x_ != other.x_ && y_ != other.y_ && z_ != other.z_;
  }

  bool operator>(const SmoothVec3& other) const noexcept {
    return x_ > other.x_ && y_ > other.y_ && z_ > other.z_;
  }

  bool operator<(const SmoothVec3& other) const noexcept {
    return x_ < other.x_ && y_ < other.y_ && z_ < other.z_;
  }

  bool operator>=(const SmoothVec3& other) const noexcept {
    return x_ >= other.x_ && y_ >= other.y_ && z_ >= other.z_;
  }

  bool operator<=(const SmoothVec3& other) const noexcept {
    return x_ <= other.x_ && y_ <= other.y_ && z_ <= other.z_;
  }

  operator glm::vec3() const noexcept { return {x_, y_, z_}; }

  SmoothFloat x() const noexcept { return x_; }

  void x(const SmoothFloat& v) noexcept { x_ = v; }

  SmoothFloat y() const noexcept { return y_; }

  void y(const SmoothFloat& v) noexcept { y_ = v; }

  SmoothFloat z() const noexcept { return z_; }

  void z(const SmoothFloat& v) noexcept { z_ = v; }

 private:
  SmoothFloat x_;
  SmoothFloat y_;
  SmoothFloat z_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_SMOOTH_VEC_H_
