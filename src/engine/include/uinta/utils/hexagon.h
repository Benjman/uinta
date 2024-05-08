#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_HEXAGON_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_HEXAGON_H_

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/lib/absl/strings.h"
#include "uinta/types.h"

namespace uinta {

struct Hexagon {
  enum class Direction : u8 {
    East,
    Northeast,
    Northwest,
    West,
    Southwest,
    Southeast,
  };

  enum class DiagonalDirection : u8 {
    Northeast,
    North,
    NorthWest,
    SouthWest,
    South,
    SouthEast,
  };

  static std::vector<Hexagon> Ring(const Hexagon& center, i32 radius) noexcept {
    static std::array<Hexagon, 6> directions = {
        Hexagon(+1, +0, -1), Hexagon(+1, -1, +0), Hexagon(+0, -1, +1),
        Hexagon(-1, +0, +1), Hexagon(-1, +1, +0), Hexagon(+0, +1, -1)};

    if (radius == 0) return {center};

    std::vector<Hexagon> result;
    result.reserve(6 * radius);

    auto hexagon =
        center +
        directions.at(static_cast<i32>(Hexagon::Direction::Southwest)) * radius;

    for (i32 i = 0; i < 6; i++) {
      for (i32 j = 0; j < radius; j++) {
        result.push_back(hexagon);
        hexagon = hexagon.neighbor(static_cast<Hexagon::Direction>(i));
      }
    }

    return result;
  }

  static std::vector<Hexagon> Spiral(const Hexagon& hexagon, i32 radius) {
    std::vector<Hexagon> result;
    result.reserve(3 * radius * (radius + 1) + 1);
    for (auto i = 1; i <= radius; i++) {
      auto ring = Ring(hexagon, i);
      std::copy(ring.begin(), ring.end(), std::back_inserter(result));
    }
    result.push_back(hexagon);
    return result;
  }

  i32 q;
  i32 r;
  i32 s;

  explicit constexpr Hexagon(i32 q = 0, i32 r = 0, i32 s = 0)
      : q(q), r(r), s(s) {
    assert(q + r + s == 0 && "Invalid hexagon coordinates");
  }

  // Other methods and operators remain the same...
  bool operator==(const Hexagon& other) const noexcept {
    return q == other.q && r == other.r && s == other.s;
  }

  bool operator!=(const Hexagon& other) const noexcept {
    return !(*this == other);
  }

  Hexagon operator+(const Hexagon& other) const noexcept {
    return Hexagon(q + other.q, r + other.r, s + other.s);
  }

  Hexagon operator-(const Hexagon& other) const noexcept {
    return Hexagon(q - other.q, r - other.r, s - other.s);
  }

  Hexagon operator*(i32 k) const noexcept {
    return Hexagon(q * k, r * k, s * k);
  }

  operator glm::vec3() const noexcept { return {q, r, s}; }

  i32 length() const noexcept {
    return (std::abs(q) + std::abs(r) + std::abs(s)) / 2;
  }

  i32 distance(const Hexagon& other) const noexcept {
    return (*this - other).length();
  }

  Hexagon neighbor(Direction direction) const {
    constexpr std::array<Hexagon, 6> directions = {
        Hexagon(+1, +0, -1), Hexagon(+1, -1, +0), Hexagon(+0, -1, +1),
        Hexagon(-1, +0, +1), Hexagon(-1, +1, +0), Hexagon(+0, +1, -1)};

    static_assert(sizeof(directions) / sizeof(Hexagon) == 6,
                  "Mismatch in number of directions");

    return *this + directions[static_cast<i32>(direction)];
  }

  Hexagon neighborDiag(DiagonalDirection direction) const {
    constexpr std::array<Hexagon, 6> directions = {
        Hexagon(+2, -1, -1), Hexagon(+1, -2, +1), Hexagon(-1, -1, +2),
        Hexagon(-2, +1, +1), Hexagon(-1, +2, -1), Hexagon(+1, +1, -2)};

    static_assert(sizeof(directions) / sizeof(Hexagon) == 6,
                  "Mismatch in number of diagonal directions");

    return *this + directions[static_cast<i32>(direction)];
  }

  glm::vec2 centerPoint(f32 radius) const noexcept {
    return glm::vec2(radius * (3.0f / 2.0f * q),
                     radius * (glm::root_three<f32>() * (r + q / 2.0f)));
  }

  std::array<glm::vec2, 6> vertexPoints(f32 radius) const noexcept {
    std::array<glm::vec2, 6> points;
    auto center = centerPoint(radius);

    for (i32 i = 0; i < 6; i++) {
      f32 angle = glm::radians(60.0f * i);
      points.at(i) = glm::vec2(center.x + radius * cos(angle),
                               center.y + radius * sin(angle));
    }

    return points;
  }

  std::array<glm::vec2, 7> vertexPointsWithCenter(f32 radius) const noexcept {
    std::array<glm::vec2, 7> points;
    points.at(0) = centerPoint(radius);
    auto vpts = vertexPoints(radius);
    for (size_t i = 0; i < vpts.size(); i++) points.at(i + 1) = vpts.at(i);
    return points;
  }
};

inline std::string to_string(const Hexagon& hexagon) noexcept {
  return StrFormat("Hexagon(%i, %i, %i)", hexagon.q, hexagon.r, hexagon.s);
}

class HexagonGrid {
 public:
  explicit HexagonGrid(f32 hexagonSize, u32 ringCount) noexcept
      : hexagonSize_(hexagonSize), ringCount_(ringCount) {}

  std::vector<Hexagon> generate() const noexcept {
    std::vector<Hexagon> result;
    result.reserve(3 * ringCount_ * (ringCount_ + 1) + 1);
    for (u32 i = 1; i <= ringCount_; i++) {
      auto ring = Hexagon::Ring(center_, i);
      std::copy(ring.begin(), ring.end(), std::back_inserter(result));
    }
    result.push_back(center_);
    return result;
  }

  f32 hexagonSize() const noexcept { return hexagonSize_; }

  u32 ringCount() const noexcept { return ringCount_; }

 private:
  f32 hexagonSize_;
  u32 ringCount_;
  Hexagon center_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_HEXAGON_H_
