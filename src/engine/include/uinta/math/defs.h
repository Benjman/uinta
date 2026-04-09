#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_DEFS_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_DEFS_H_

#include <glm/gtc/constants.hpp>
#include <limits>
#include <numbers>

#include "uinta/types.h"

namespace uinta {

constexpr f32 epsilon_f = std::numeric_limits<f32>::epsilon();
constexpr f32 rootThree_f = glm::root_three<f32>();
constexpr f32 PI = std::numbers::pi_v<float>;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_DEFS_H_
