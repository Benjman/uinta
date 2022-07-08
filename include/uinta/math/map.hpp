#ifndef UINTA_MATH_MAP_HPP
#define UINTA_MATH_MAP_HPP

#include <algorithm>
#include <glm/mat4x4.hpp>

namespace uinta {

void updateViewMatrix(glm::mat4 &view, const glm::vec3 pos, const float pitch, const float yaw);

inline float map0to1Range(const float value, const float min, const float max) { return (value - min) / (max - min); }

inline float map0to1RangeClamped(const float value, const float min, const float max) {
  return std::clamp(map0to1Range(value, min, max), 0.0f, 1.0f);
}

} // namespace uinta

#endif // UINTA_MATH_MAP_HPP
