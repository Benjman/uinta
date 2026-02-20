#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_HEX_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_HEX_H_

/// Usage:
/// ```cpp
///       Hex origin(0,0);
///       auto radius = 2;
///       auto hexagonSize = 1.0;
///       auto fillPercentage = 1.0;
///       HexGroup group(origin, radius, hexagonSize, fillPercentage);
///       Mesh fill = Mesh::Hexagon::Fill(&group);
///       Mesh outline = Mesh::Hexagon::Outline(&group);
/// ```

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/color.h"
#include "uinta/flags.h"
#include "uinta/math/defs.h"

namespace uinta {

struct Hex final {
  using value_type = i32;

  value_type q, r;

  Hex(value_type q, value_type r) noexcept : q(q), r(r) {}

  Hex(const glm::ivec2& vec) noexcept : Hex(vec.x, vec.y) {}

  bool operator==(Hex rhs) const noexcept { return q == rhs.q && r == rhs.r; }

  bool operator!=(Hex rhs) const noexcept { return !(*this == rhs); }

  operator glm::ivec2() const noexcept { return {q, r}; }

  operator glm::vec2() const noexcept { return {q, r}; }

  Hex operator=(const glm::ivec2& other) const noexcept {
    return Hex(other.x, other.y);
  }

  Hex operator+(Hex rhs) const noexcept { return Hex(q + rhs.q, r + rhs.r); }

  Hex operator-(Hex rhs) const noexcept { return Hex(q - rhs.q, r - rhs.r); }

  bool operator<(const Hex& other) const noexcept {
    return std::tie(q, r) < std::tie(other.q, other.r);
  }

  friend std::ostream& operator<<(std::ostream& os, const Hex& obj) {
    os << "Hex(" << obj.q << ", " << obj.r << ")";
    return os;
  }

  [[nodiscard]] Hex scale(value_type scale) const noexcept {
    return Hex(q * scale, r * scale);
  }

  Hex direction(value_type direction) const noexcept {
    static const std::array<Hex, 6> directions(
        {Hex(1, 0), Hex(1, -1), Hex(0, -1), Hex(-1, 0), Hex(-1, 1), Hex(0, 1)});
    assert(direction >= 0 && direction < 6 &&
           "Direction must be between 0 and 5.");
    return directions.at(direction);
  }

  Hex diagonal(value_type direction) const noexcept {
    static const std::array<Hex, 6> diagonals({Hex(2, -1), Hex(1, -2),
                                               Hex(-1, -1), Hex(-2, 1),
                                               Hex(-1, 2), Hex(1, 1)});
    assert(direction >= 0 && direction < 6 &&
           "Direction must be between 0 and 5.");
    return diagonals.at(direction);
  }

  Hex neighbor(value_type direction) const noexcept {
    return *this + this->direction(direction);
  }

  value_type length() const noexcept {
    return static_cast<value_type>((abs(q) + abs(r)) / 2.0);
  }

  value_type distance(Hex rhs) const noexcept { return (*this - rhs).length(); }

  glm::vec2 centerPoint(f32 size = 1) const noexcept {
    return {size * (1.5 * q), size * (rootThree_f * 0.5 * q + rootThree_f * r)};
  }

  std::array<glm::vec2, 6> generatePoints(f32 size = 1,
                                          f32 fill = 1) const noexcept {
    std::array<glm::vec2, 6> result;
    auto center = centerPoint(size);
    size *= fill;
    for (auto i = 0; i < 6; i++) {
      const auto angle = glm::radians<f64>(60 * i);
      result.at(i) = center + glm::vec2(size * cos(angle), size * sin(angle));
    }
    return result;
  }
};

struct HexGroup final {
  struct Flags final {
    using value_type = i32;

    static constexpr value_type RingMask = 1 << 0;
    static constexpr value_type OrientationMask = 1 << 1;
    static constexpr value_type LayoutMask = 0b11 << 2;

    static constexpr value_type LayoutHexagonMask = 0b0 << 2;
    static constexpr value_type LayoutRectangleMask = 0b1 << 2;
    static constexpr value_type LayoutRhombusMask = 0b10 << 2;
    static constexpr value_type LayoutTriangleMask = 0b11 << 2;

    static constexpr value_type DefaultFlags = OrientationMask;

    AtomicFlagsOperations(DefaultFlags);

    bool isRing() const noexcept { return flags_ & RingMask; }

    void isRing(bool v) noexcept {
      flags_ &= ~RingMask;
      if (v) flags_ |= RingMask;
    }

    bool isFlatTop() const noexcept { return flags_ & OrientationMask; }
    void isFlatTop(bool v) noexcept {
      flags_ &= ~OrientationMask;
      if (v) flags_ |= OrientationMask;
    }

    bool isPointyTop() const noexcept { return !isFlatTop(); }
    void isPointyTop(bool v) noexcept { isFlatTop(!v); }

    bool isLayoutHexagon() const noexcept {
      return (flags_ & LayoutMask) == LayoutHexagonMask;
    }
    void isLayoutHexagon(bool v) noexcept { setLayout(LayoutHexagonMask, v); }

    bool isLayoutRectangle() const noexcept {
      return (flags_ & LayoutMask) == LayoutRectangleMask;
    }
    void isLayoutRectangle(bool v) noexcept {
      setLayout(LayoutRectangleMask, v);
    }

    bool isLayoutRhombus() const noexcept {
      return (flags_ & LayoutMask) == LayoutRhombusMask;
    }
    void isLayoutRhombus(bool v) noexcept { setLayout(LayoutRhombusMask, v); }

    bool isLayoutTriangle() const noexcept {
      return (flags_ & LayoutMask) == LayoutTriangleMask;
    }
    void isLayoutTriangle(bool v) noexcept { setLayout(LayoutTriangleMask, v); }

   private:
    std::atomic<value_type> flags_;

    void setLayout(value_type mask, bool v) noexcept {
      flags_ &= ~LayoutMask;
      if (v) flags_ |= mask;
    }
  };

  Hex origin;
  Hex::value_type radius;
  f32 size;
  f32 fillPercentage;
  Flags flags = Flags::DefaultFlags;

  HexGroup(Hex origin = {0, 0}, Hex::value_type radius = 2, f32 size = 1,
           f32 fillPercentage = 1, Flags flags = Flags::DefaultFlags) noexcept
      : origin(origin),
        radius(radius),
        size(size),
        fillPercentage(fillPercentage),
        flags(flags) {}

  bool isRing() const noexcept { return flags.isRing(); }

  bool isSpiral() const noexcept { return !isRing(); }

  Color fillColor(Hex hex) const noexcept {
    if (auto itr = colors_.find(hex); itr != colors_.end()) {
      return itr->second;
    }
    constexpr auto DefaultColor = color::BlueGray50;
    return DefaultColor;
  }

  Color outlineColor(Hex) const noexcept {
    constexpr auto DefaultColor = color::BlueGray300;
    return DefaultColor;
  }

  std::vector<Hex> generateHexagons() const noexcept {
    if (flags.isLayoutHexagon()) {
      if (flags.isRing()) {
        return Ring(radius);
      } else {
        return Spiral();
      }
    } else if (flags.isLayoutRhombus()) {
      return Rhombus();
    }
    assert(false && "Unimplemented layout");
  }

  size_t hexCount() const noexcept {
    if (flags.isLayoutHexagon()) {
      return isRing() ? 6 * radius : 1 + 3 * radius * (radius + 1);
    } else if (flags.isLayoutRhombus()) {
      return std::pow(radius * 2 + 1, 2);
    }
    assert(false && "Unimplemented layout");
  }

  glm::vec2 hexToPoint(Hex hex) const noexcept {
    if (flags.isFlatTop()) {
      return {
          size * (3.0 / 2.0 * hex.q),
          size * (rootThree_f / 2.0 * hex.q + rootThree_f * hex.r),
      };
    } else {
      return {
          size * (rootThree_f * hex.q + rootThree_f / 2.0 * hex.r),
          size * (3.0 / 2.0 * hex.r),
      };
    }
  }

  Hex pointToHex(glm::vec2 point) const noexcept {
    f32 q, r;
    if (flags.isFlatTop()) {
      q = (2.0 / 3.0 * point.x) / size;
      r = (-1.0 / 3.0 * point.x + (rootThree_f / 3.0) * point.y) / size;
    } else {
      q = (rootThree_f * point.x - 1.0 / 3.0 * point.y) / size;
      r = (2.0 / 3.0 * point.y) / size;
    }
    q = std::round(q);
    r = std::round(r);
    using value_type = Hex::value_type;
    return Hex(static_cast<value_type>(q), static_cast<value_type>(r));
  }

  void setColor(Hex hex, Color color) noexcept { colors_.emplace(hex, color); }

 private:
  std::map<Hex, Color> colors_;

  std::vector<Hex> Ring(Hex::value_type radius) const noexcept {
    assert(radius > 0 && "Radius must be greater than zero.");
    std::vector<Hex> result;
    result.reserve(hexCount());
    Hex hex = origin + origin.direction(4).scale(radius);
    for (auto i = 0; i < 6; i++) {
      for (auto ii = 0; ii < radius; ii++) {
        result.push_back(hex);
        hex = hex + hex.direction(i);
      }
    }
    return result;
  }

  std::vector<Hex> Spiral() const noexcept {
    assert(radius > 0 && "Radius must be greater than zero.");
    std::vector<Hex> result;
    result.reserve(hexCount());
    result.emplace_back(origin);
    for (auto i = 1; i <= radius; i++) {
      auto ring = Ring(i);
      std::copy(ring.begin(), ring.end(), std::back_inserter(result));
    }
    return result;
  }

  std::vector<Hex> Rhombus() const noexcept {
    std::vector<Hex> result;
    result.reserve(hexCount());
    for (i32 i = -radius; i <= radius; i++) {
      for (i32 ii = -radius; ii <= radius; ii++) {
        result.push_back(Hex(i, ii));
      }
    }
    return result;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_HEX_H_
