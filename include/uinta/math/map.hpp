#ifndef UINTA_MATH_MAP_HPP
#define UINTA_MATH_MAP_HPP

#include <uinta/math/fwd.hpp>

namespace uinta {

inline f32 map0to1Range(const f32 value, const f32 min, const f32 max) {
  return (value - min) / (max - min);
}

inline f32 map0to1RangeClamped(const f32 value, const f32 min, const f32 max) {
  return clamp(map0to1Range(value, min, max), 0.0, 1.0);
}

inline f32 clamp(f32 value, f32 min, f32 max) {
  if (value >= min && value <= max) return value;
  return value < min ? min : max;
}

}  // namespace uinta

#endif  // UINTA_MATH_MAP_HPP
