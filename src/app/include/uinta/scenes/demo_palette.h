#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_PALETTE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_PALETTE_H_

#include <array>

#include "glm/ext/vector_float4.hpp"

namespace uinta {

struct Palettes final {
  using value_type = std::array<glm::vec4, 5>;

  static constexpr value_type TheGrassIsAlwaysGreener = {
      glm::vec4(58.0 / 255.0, 57.0 / 255.0, 66.0 / 255.0, 1.0),
      glm::vec4(255.0 / 255.0, 233.0 / 255.0, 189.0 / 255.0, 1.0),
      glm::vec4(209.0 / 255.0, 202.0 / 255.0, 147.0 / 255.0, 1.0),
      glm::vec4(177.0 / 255.0, 188.0 / 255.0, 122.0 / 255.0, 1.0),
      glm::vec4(130.0 / 255.0, 168.0 / 255.0, 93.0 / 255.0, 1.0),
  };

  static constexpr value_type PrimaryPalette() noexcept {
    return TheGrassIsAlwaysGreener;
  }
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_PALETTE_H_
