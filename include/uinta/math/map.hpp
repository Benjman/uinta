#ifndef UINTA_MATH_MAP_HPP
#define UINTA_MATH_MAP_HPP

#include <uinta/math/fwd.hpp>

namespace uinta {

inline float map0to1Range(const float value, const float min, const float max) {
  return (value - min) / (max - min);
}

inline float map0to1RangeClamped(const float value, const float min, const float max) {
  return clamp(map0to1Range(value, min, max), 0.0, 1.0);
}

inline float clamp(float value, float min, float max) {
  if (value >= min && value <= max) return value;
  return value < min ? min : max;
}

}  // namespace uinta

#endif  // UINTA_MATH_MAP_HPP
