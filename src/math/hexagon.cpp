#include <algorithm>
#include <array>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <uinta/logging.hpp>
#include <uinta/math/fwd.hpp>
#include <uinta/math/hexagon.hpp>
#include <uinta/math/interpolation.hpp>
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

void hexagon_pack(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals,
                  const std::vector<glm::vec3>& colors, f32* const vtxBuffer, u32* const idxBuffer, u32& idxOffset) {
  // Offsets describing position in the buffer to pack each part
  static constexpr size_t PointOffset = 0;
  static constexpr size_t NormalOffset = 3;
  static constexpr size_t ColorOffset = 6;
  static constexpr size_t OffsetSize = PointOffset + NormalOffset + ColorOffset;

  if (points.size() != normals.size())
    throw std::range_error("Mismatch between number of points (" + std::to_string(points.size()) + ") and number of normals (" +
                           std::to_string(normals.size()) + ").");

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
    vtxBuffer[index + NormalOffset] = normals[i].x;
    vtxBuffer[index + NormalOffset + 1] = normals[i].y;
    vtxBuffer[index + NormalOffset + 2] = normals[i].z;
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

std::vector<glm::vec3> radial_hexagons(const glm::vec3& origin, u32 rings, f32 radius) {
  const auto spacing = hex_spacing(radius);

  // Generate a collection of cube-based coordinates for the range specified
  const auto spiral = cube_spiral(origin, rings);

  u32 index = 0;
  auto points = std::vector<glm::vec3>(hexagon_count(rings) * VerticesPerHex);
  for (const auto& hex : spiral) {
    // Convert from cube coordinates to axial coordinates
    auto offset = cube_to_axial(hex) * spacing;

    // Staggering
    if (hex.z % 2) offset.x += spacing.x * 0.5;

    // Generate points for the hexagon
    const auto hexPoints = hex_points(offset, radius);

    // Move the hexagon points into the main collection
    std::copy(hexPoints.begin(), hexPoints.end(), points.begin() + index);

    // Offset the index
    index += VerticesPerHex;
  };

  return points;
}

glm::ivec3 cube_neighbor(glm::ivec3& cube, hex_direction direction) {
  return cube + cube_directions[static_cast<i32>(direction)];
}

u32 hexagon_count(u32 rings) {
  return 1 + 3 * rings * (rings + 1);
}

std::vector<glm::ivec3> cube_ring(const glm::ivec3& origin, u32 radius) {
  auto result = std::vector<glm::ivec3>(6 * radius);
  auto hex = origin + cube_directions[static_cast<i32>(hex_direction::SouthWest)] * static_cast<i32>(radius);
  for (u32 i = 0; i < 6; ++i) {
    std::generate_n(result.begin() + i * radius, radius, [&hex, i]() {
      auto current_hex = hex;
      hex = cube_neighbor(hex, static_cast<hex_direction>(i));
      return current_hex;
    });
  }
  return result;
}

std::vector<glm::ivec3> cube_spiral(const glm::ivec3& origin, u32 radius) {
  auto result = std::vector<glm::ivec3>(hexagon_count(radius));
  for (u32 r = 1, count = 0; r <= static_cast<u32>(radius); ++r) {
    const auto ring = cube_ring(origin, r);
    std::copy(ring.begin(), ring.end(), result.begin() + count);
    count += ring.size();
  }
  return result;
}

glm::ivec3 axial_to_cube(const glm::vec2& hex) {
  const auto q = static_cast<i32>(hex.x);
  const auto r = static_cast<i32>(hex.y);
  const auto s = -q - r;
  return {q, r, s};
}

glm::vec2 cube_to_axial(const glm::ivec3& cube) {
  const auto col = cube.x + (cube.z - (cube.z & 1)) / 2;
  const auto row = cube.z;
  return {col, row};
}

f32 cube_distance(const glm::vec3& a, const glm::vec3& b) {
  const auto v = a - b;
  return (fabs(v.x) + fabs(v.y) + fabs(v.z)) / 2.0f;
}

glm::ivec2 world_to_axial(const glm::vec2& pos, const glm::vec2& origin, f32 size) {
  // TODO: This isn't exactly correct. See https://www.redblobgames.com/grids/hexagons/#hex-to-pixel

  // normalize the coordinates to the grid origin
  const auto xNorm = pos.x - origin.x;
  const auto yNorm = pos.y - origin.y;

  // convert to fractional axial coordinates
  const auto qFractional = (glm::root_three<f32>() / 3 * xNorm - 1.0 / 3 * yNorm) / size;
  const auto rFractional = (2.0 / 3 * yNorm) / size;

  // round to the nearest axial coordinates
  const auto q = std::round(qFractional);
  const auto r = std::round(rFractional);
  const auto s = -q - r;

  // use cube coordinates to handle rounding errors
  const auto qDiff = std::abs(q - qFractional);
  const auto rDiff = std::abs(r - rFractional);
  const auto sDiff = std::abs(s + qFractional + rFractional);

  if (qDiff > rDiff && qDiff > sDiff)
    return {-r - s, r};
  else if (rDiff > sDiff)
    return {q, -q - s};
  else
    return {q, r};
}

std::vector<glm::vec3> hex_normals(const std::vector<glm::vec3>& points) {
  static constexpr size_t CenterIndex = 0;

  std::vector<glm::vec3> normals(points.size());

  if (points.size() % VerticesPerHex) throw std::invalid_argument("Points vector size must be a multiple of VerticesPerHex!");

  for (size_t i = 0; i < points.size(); i += VerticesPerHex) {
    const auto& center = points[i];
    auto& centerNormal = normals[i];
    centerNormal = glm::vec3(0);

    for (u32 j = 1; j < VerticesPerHex; ++j) {
      auto v1Index = i + j;
      auto v2Index = i + (j % 6) + 1;

      // Move to the next point if we land on the center index;
      if (v1Index == CenterIndex) v1Index++;
      if (v2Index == CenterIndex) v2Index++;

      const auto normal = glm::triangleNormal(points[v1Index], points[v2Index], center);

      normals[j + i] = normal;
      centerNormal += normal;
    }

    centerNormal = glm::normalize(centerNormal);
  }
  return normals;
}

void hex_set_heights(std::vector<glm::vec3>& points, const Buffer2d<f32>& heightMap, f32 scale) {
  for (auto& point : points) {
    const glm::vec2 coord = {point.x + static_cast<f32>(heightMap.size.x) / 2.0f,
                             point.z + static_cast<f32>(heightMap.size.y) / 2.0f};
    point.y = bilinear(&heightMap, heightMap.size, coord) * scale;
  }
}

}  // namespace uinta
