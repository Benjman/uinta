#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_

#include <cmath>

#include "uinta/math/defs.h"
#include "uinta/types.h"

namespace uinta {

inline f32 easeInSine(f32 x) noexcept { return 1.0 - std::cos((PI * x) / 2.0); }

inline f32 easeOutSine(f32 x) noexcept { return std::sin((PI * x) / 2.0); }

inline f32 easeInOutSine(f32 x) noexcept {
  return -std::cos((PI * x) - 1.0) / 2.0;
}

inline f32 easeInQuad(f32 x) noexcept { return x * x; }

inline f32 easeOutQuad(f32 x) noexcept { return 1.0 - (1.0 - x) * (1.0 - x); }

inline f32 easeInOutQuad(f32 x) noexcept {
  return x < 0.5 ? 2.0 * x * x : 1.0 - std::pow(-2.0 * x + 2.0, 2.0) / 2.0;
}

inline f32 easeInCubic(f32 x) noexcept { return x * x * x; }

inline f32 easeOutCubic(f32 x) noexcept { return 1.0 - std::pow(1.0 - x, 3); }

inline f32 easeInOutCubic(f32 x) noexcept {
  return x < 0.5 ? 4.0 * x * x * x : 1.0 - std::pow(-2.0 * x + 2.0, 3.0) / 2.0;
}

inline f32 easeInQuart(f32 x) noexcept { return x * x * x * x; }

inline f32 easeOutQuart(f32 x) noexcept { return 1.0 - std::pow(1.0 - x, 4); }

inline f32 easeInOutQuart(f32 x) noexcept {
  return x < 0.5 ? 8.0 * x * x * x * x
                 : 1.0 - std::pow(-2.0 * x + 2.0, 4.0) / 2.0;
}

inline f32 easeInQuint(f32 x) noexcept { return x * x * x * x * x; }

inline f32 easeOutQuint(f32 x) noexcept { return 1.0 - std::pow(1.0 - x, 5.0); }

inline f32 easeInOutQuint(f32 x) noexcept {
  return x < 0.5 ? 16.0 * x * x * x * x * x
                 : 1.0 - std::pow(-2.0 * x + 2.0, 5.0) / 2.0;
}

inline f32 easeInExpo(f32 x) noexcept {
  return x == 0.0 ? 0.0 : std::pow(2.0, 10.0 * x - 10.0);
}

inline f32 easeOutExpo(f32 x) noexcept {
  return x == 1.0 ? 1.0 : 1.0 - std::pow(2, -10 * x);
}

inline f32 easeInOutExpo(f32 x) noexcept {
  return x == 0.0   ? 0.0
         : x == 1.0 ? 1.0
         : x < 0.5  ? std::pow(2.0, 20.0 * x - 10.0) / 2.0
                    : (2.0 - std::pow(2.0, -20.0 * x + 10.0)) / 2.0;
}

inline f32 easeInCirc(f32 x) noexcept {
  return 1.0 - std::sqrt(1.0 - std::pow(x, 2.0));
}

inline f32 easeOutCirc(f32 x) noexcept {
  return std::sqrt(1.0 - std::pow(x - 1.0, 2.0));
}

inline f32 easeInOutCirc(f32 x) noexcept {
  return x < 0.5 ? (1.0 - std::sqrt(1.0 - std::pow(2.0 * x, 2.0))) / 2.0
                 : (std::sqrt(1.0 - std::pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
}

inline f32 easeInBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158;
  constexpr f32 c3 = c1 + 1.0;
  return c3 * x * x * x - c1 * x * x;
}

inline f32 easeOutBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158;
  constexpr f32 c3 = c1 + 1.0;
  return 1.0 + c3 + std::pow(x - 1.0, 3.0) + c1 * std::pow(x - 1.0, 2.0);
}

inline f32 easeInOutBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158;
  constexpr f32 c2 = c1 * 1.525;
  return x < 0.5 ? (std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0
                 : (std::pow(2.0 * x - 2.0, 2.0) *
                        ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) +
                    2.0) /
                       2.0;
}

inline f32 easeInElastic(f32 x) noexcept {
  constexpr f32 c4 = (2.0 * PI) / 3.0;
  return x == 0.0   ? 0
         : x == 1.0 ? 1
                    : -std::pow(2.0, 10.0 * x - 10.0) *
                          std::sin((x * 10.0 - 10.75) * c4);
}

inline f32 easeOutElastic(f32 x) noexcept {
  constexpr f32 c4 = (2.0 * PI) / 3.0;
  return x == 0.0 ? 0
         : x == 1.0
             ? 1
             : std::pow(2.0, -10.0 * x) * std::sin((x * 10.0 - 0.75) * c4) +
                   1.0;
}

inline f32 easeInOutElastic(f32 x) noexcept {
  constexpr f32 c5 = (2.0 * PI) / 4.5;
  return x == 0.0   ? 0.0
         : x == 1.0 ? 1.0
         : x < 0.5  ? -(std::pow(2.0, 20.0 * x - 10.0) *
                       std::sin((20.0 * x - 11.125) * c5)) /
                         2.0
                   : (std::pow(2.0, -20.0 * x + 10.0) *
                      std::sin((20.0 * x - 11.125) * c5)) /
                             2.0 +
                         1.0;
}

inline f32 easeOutBounce(f32 x) noexcept {
  const f32 kN1 = 7.5625;
  const f32 kD1 = 2.75;
  if (x < 1.0 / kD1) {
    return kN1 * x * x;
  } else if (x < 2.0 / kD1) {
    x -= 1.5 / kD1;
    return kN1 * x * x + 0.75;
  } else if (x < 2.5 / kD1) {
    x -= 2.25 / kD1;
    return kN1 * x * x + 0.9375;
  } else {
    x -= 2.625 / kD1;
    return kN1 * x * x + 0.984375;
  }
}

inline f32 easeInBounce(f32 x) noexcept { return 1.0 - easeOutBounce(1.0 - x); }

inline f32 easeInOutBounce(f32 x) noexcept {
  return x < 0.5 ? (1.0 - easeOutBounce(1.0 - 2.0 * x)) / 2.0
                 : (1.0 + easeOutBounce(2.0 * x - 1.0)) / 2.0;
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_
