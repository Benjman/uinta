#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_H_

#include <limits>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/types.h"

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
    current_ += (target_ - current_) * agility_ * dt;
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
    return std::abs(current_ - other.current_) <=
           std::numeric_limits<f32>::epsilon();
  }

  bool operator!=(const SmoothFloat& other) const noexcept {
    return std::abs(current_ - other.current_) !=
           std::numeric_limits<f32>::epsilon();
  }

  bool operator>(const SmoothFloat& other) const noexcept {
    return std::abs(current_ - other.current_) >
           std::numeric_limits<f32>::epsilon();
  }

  bool operator<(const SmoothFloat& other) const noexcept {
    return std::abs(current_ - other.current_) <
           std::numeric_limits<f32>::epsilon();
  }

  bool operator>=(const SmoothFloat& other) const noexcept {
    return std::abs(current_ - other.current_) >=
           std::numeric_limits<f32>::epsilon();
  }

  bool operator<=(const SmoothFloat& other) const noexcept {
    return std::abs(current_ - other.current_) <=
           std::numeric_limits<f32>::epsilon();
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

constexpr glm::vec3 WorldForward = glm::vec3(0, 0, -1);
constexpr glm::vec3 WorldRight = glm::vec3(1, 0, 0);
constexpr glm::vec3 WorldUp = glm::vec3(0, 1, 0);
constexpr glm::vec3 WorldHorizontal = WorldRight + WorldForward;

inline glm::vec3 getForward(f32 pitch, f32 yaw) noexcept;
inline f32 getPlaneInterceptDist(const glm::vec3& planePos,
                                 const glm::vec3& planeNormal,
                                 const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection) noexcept;
inline glm::vec3 getPlaneInterceptPoint(const glm::vec3& planePos,
                                        const glm::vec3& planeNormal,
                                        const glm::vec3& rayPos,
                                        const glm::vec3& rayDirection) noexcept;
inline glm::vec3 getPointOnRay(const glm::vec3& position,
                               const glm::vec3& direction, f32 dist) noexcept;
inline glm::vec3 getRight(f32 yaw) noexcept;
inline glm::vec3 getUp(f32 pitch, f32 yaw) noexcept;
inline glm::vec3 getUp(const glm::vec3& forward,
                       const glm::vec3& right) noexcept;
inline glm::vec3 getWorldRay(const glm::vec2& cursorPos,
                             const glm::vec2& viewportSize,
                             const glm::mat4& view,
                             const glm::mat4& proj) noexcept;

inline glm::vec3 getForward(f32 pitch, f32 yaw) noexcept {
  auto pitchRad = glm::radians(pitch);
  auto yawRad = glm::radians(yaw);
  return {cos(pitchRad) * sin(yawRad), -sin(pitchRad),
          cos(pitchRad) * cos(yawRad)};
}

inline f32 getPlaneInterceptDist(const glm::vec3& planePos,
                                 const glm::vec3& planeNormal,
                                 const glm::vec3& rayPos,
                                 const glm::vec3& rayDirection) noexcept {
  auto denom = glm::dot(planeNormal, rayDirection);
  if (std::abs(denom) <= 1e-6) return 0;
  return glm::dot(planePos - rayPos, planeNormal) / denom;
}

inline glm::vec3 getPlaneInterceptPoint(
    const glm::vec3& planePos, const glm::vec3& planeNormal,
    const glm::vec3& rayPos, const glm::vec3& rayDirection) noexcept {
  auto dist =
      getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDirection);
  if (dist <= 0) return glm::vec3(INFINITY);
  return getPointOnRay(rayPos, rayDirection, dist);
}

inline glm::vec3 getPointOnRay(const glm::vec3& position,
                               const glm::vec3& direction, f32 dist) noexcept {
  return position + direction * dist;
}

inline glm::vec3 getRight(f32 yaw) noexcept {
  auto yawRad = glm::radians(yaw);
  return {cos(yawRad), 0, -sin(yawRad)};
}

inline glm::vec3 getUp(f32 pitch, f32 yaw) noexcept {
  return getUp(getForward(pitch, yaw), getRight(yaw));
}

inline glm::vec3 getUp(const glm::vec3& forward,
                       const glm::vec3& right) noexcept {
  return glm::cross(forward, right);
}

inline glm::vec3 getWorldRay(const glm::vec2& cursorPos,
                             const glm::vec2& viewportSize,
                             const glm::mat4& view,
                             const glm::mat4& proj) noexcept {
  glm::vec3 ndc = {(2 * cursorPos.x) / viewportSize.x - 1,
                   1 - (2 * cursorPos.y) / viewportSize.y, 1};
  glm::vec4 clip = {ndc.x, ndc.y, -1, 1};
  glm::vec4 eye = {glm::vec2(glm::inverse(proj) * clip), -1, 0};
  return glm::normalize(glm::inverse(view) * eye);
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_H_
