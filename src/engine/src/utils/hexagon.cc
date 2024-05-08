#include "uinta/utils/hexagon.h"

#include <algorithm>
#include <iterator>

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/math/defs.h"

namespace uinta {

size_t HexagonCount(size_t ringCount) noexcept {
  return 1 + 3 * ringCount * (ringCount + 1);
}

std::array<glm::vec2, 7> HexagonPoints(const Hexagon& hexagon,
                                       f32 radius) noexcept {
  assert(hexagon.q_ + hexagon.r_ + hexagon.s_ == 0 &&
         "The sum of q, r, and s must equal 0 for valid cube coordinates.");

  LOG(INFO) << StrFormat("%+i, %+i, %+i", hexagon.q_, hexagon.r_, hexagon.s_);

  std::array<glm::vec2, 7> points;
  f32 cx = rootThree_f * (hexagon.q_ + hexagon.r_ / 2.0) * radius;
  f32 cy = 1.5 * hexagon.r_ * radius;

  auto spacing = HexagonSpacing(radius);
  if (hexagon.r_ % 2 != 0) cx += spacing.x * 0.5;

  for (i32 i = 0; i < 6; i++) {
    // TODO: Precompute angles
    f32 angle = PI / 3 * i;
    points.at(i) = {cx + radius * cos(angle), cy + radius * sin(angle)};
  }

  points.at(6) = {cx + radius, cy};

  return points;
}

std::vector<Hexagon> HexagonCubeRing(Hexagon center, size_t radius) noexcept {
  constexpr auto HexDirections = 6;
  std::vector<Hexagon> result;
  result.reserve(HexDirections * radius);
  auto neighbor =
      HexagonCubeDirections.at(static_cast<i32>(HexagonDirection::SouthWest));
  auto hexagon = center + neighbor * radius;
  for (size_t i = 0; i < HexDirections; i++) {
    std::generate_n(std::back_inserter(result), radius, [&hexagon, i]() {
      auto current = hexagon;
      hexagon = HexagonCubeNeighbor(hexagon,
                                    static_cast<Direction>(i % HexDirections));
      return current;
    });
  }

  return result;
}

glm::vec2 HexagonSpacing(f32 radius) noexcept {
  return {rootThree_f * radius, 3.0 / 2.0 * radius};
}

}  // namespace uinta
