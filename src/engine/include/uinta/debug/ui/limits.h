#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_LIMITS_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_LIMITS_H_

#include <limits>

#include "uinta/types.h"

namespace uinta {

constexpr struct {
  f32 min = -std::numeric_limits<f32>::max();
  f32 zero = 0;
  f32 one = 1.0;
  f32 one_tenth = 0.1;
  f32 twenty = 20;
  f32 threeSixty = 360;
  f32 max = std::numeric_limits<f32>::max();
} limits;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_LIMITS_H_
