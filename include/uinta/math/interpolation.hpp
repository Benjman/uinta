#ifndef UINTA_MATH_INTERPOLATION_H
#define UINTA_MATH_INTERPOLATION_H

#include <uinta/types.h>

#include <glm/vec2.hpp>

namespace uinta {

inline f32 bilinear(const f32* const buffer, const glm::ivec2& size, const glm::vec2 coord) {
  const auto bottomLeft = glm::ivec2(static_cast<i32>(floor(coord.x)), static_cast<i32>(floor(coord.y)));
  const auto topRight = glm::ivec2(static_cast<i32>(ceil(coord.x)), static_cast<i32>(ceil(coord.y)));

  if (bottomLeft.x < 0 || bottomLeft.x >= size.x || bottomLeft.y < 0 || bottomLeft.y >= size.y || topRight.x < 0 ||
      topRight.x >= size.x || topRight.y < 0 || topRight.y >= size.y)
    return 0.0f;

  const f32 x_ratio = coord.x - bottomLeft.x;
  const f32 y_ratio = coord.y - bottomLeft.y;

  auto top = (1 - x_ratio) * buffer[topRight.y * size.x + bottomLeft.x] + x_ratio * buffer[topRight.y * size.x + topRight.x];
  auto bottom =
      (1 - x_ratio) * buffer[bottomLeft.y * size.x + bottomLeft.x] + x_ratio * buffer[bottomLeft.y * size.x + topRight.x];

  return (1 - y_ratio) * bottom + y_ratio * top;
}

}  // namespace uinta

#endif  // UINTA_MATH_INTERPOLATION_H
