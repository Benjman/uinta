#include <array>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <uinta/math/fwd.hpp>
#include <uinta/math/hexagon.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

glm::vec3 hex_point(const glm::vec2& origin, f32 radius, u32 index) {
  constexpr f32 hexDegreeStep = 60;
  constexpr f32 hexPointyTopOffset = 30;
  const auto angle_deg = hexDegreeStep * index - hexPointyTopOffset;
  const auto angle_rad = glm::radians<f64>(angle_deg);
  return {origin.x + radius * cos(angle_rad), 0, origin.y + radius * sin(angle_rad)};
}

std::array<glm::vec3, VerticesPerHex> hex_points(const glm::vec2& origin, f32 radius) {
  std::array<glm::vec3, VerticesPerHex> points;
  points[0] = glm::vec3(origin.x, 0, origin.y);
  for (size_t i = 1; i < points.size(); ++i) points.at(i) = hex_point(origin, radius, i);
  return points;
}

glm::vec2 hex_spacing(f32 radius) {
  return {glm::root_three<f32>() * radius, 3.0 / 2.0 * radius};
}

void hexagon_pack(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& colors, f32* const vtxBuffer,
                  u32* const idxBuffer, u32& idxOffset) {
  // Offsets describing position in the buffer to pack each part
  static constexpr size_t PointOffset = 0;
  static constexpr size_t NormalOffset = 3;
  static constexpr size_t ColorOffset = 6;
  static constexpr size_t OffsetSize = PointOffset + NormalOffset + ColorOffset;

  if (!colors.size()) throw std::range_error("At least one color is required!");

  const auto dataSize = points.size();
  size_t index = 0;

  // Pack vertex positions into the buffer
  for (size_t i = 0; i < dataSize; ++i) {
    index = i * OffsetSize;
    vtxBuffer[index + PointOffset] = points[i].x;
    vtxBuffer[index + PointOffset + 1] = points[i].y;
    vtxBuffer[index + PointOffset + 2] = points[i].z;
  }

  // Pack surface normals into the buffer
  for (size_t i = 0; i < dataSize; ++i) {
    index = i * OffsetSize;
    vtxBuffer[index + NormalOffset] = -WORLD_UP.x;
    vtxBuffer[index + NormalOffset + 1] = -WORLD_UP.y;
    vtxBuffer[index + NormalOffset + 2] = -WORLD_UP.z;
  }

  // Pack colors into the buffer
  glm::vec3 color;
  for (size_t i = 0; i < dataSize; ++i) {
    if (!(i % VerticesPerHex)) color = colors[(i / VerticesPerHex) % colors.size()];
    index = i * OffsetSize;
    vtxBuffer[index + ColorOffset] = color.r;
    vtxBuffer[index + ColorOffset + 1] = color.g;
    vtxBuffer[index + ColorOffset + 2] = color.b;
  }

  // Pack the index buffer
  static constexpr std::array<u32, IndicesPerHex> indices = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1};
  const auto hexCount = dataSize / VerticesPerHex;
  for (size_t i = 0; i < hexCount; ++i) {
    for (size_t j = 0; j < IndicesPerHex; ++j) idxBuffer[j + i * IndicesPerHex] = indices.at(j) + idxOffset;
    idxOffset += VerticesPerHex;
  }
}

}  // namespace uinta
