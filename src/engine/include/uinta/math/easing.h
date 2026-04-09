#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_

#include <cmath>

#include "uinta/math/defs.h"
#include "uinta/types.h"

namespace uinta {

inline f32 easeInSine(f32 x) noexcept {
  return 1.0f - std::cos((PI * x) / 2.0f);
}

inline f32 easeOutSine(f32 x) noexcept { return std::sin((PI * x) / 2.0f); }

inline f32 easeInOutSine(f32 x) noexcept {
  return -std::cos((PI * x) - 1.0f) / 2.0f;
}

inline f32 easeInQuad(f32 x) noexcept { return x * x; }

inline f32 easeOutQuad(f32 x) noexcept {
  return 1.0f - static_cast<f32>((1.0f - x) * (1.0f - x));
}

inline f32 easeInOutQuad(f32 x) noexcept {
  return x < 0.5f ? 2.0f * x * x
                  : 1.0f - static_cast<f32>(std::pow((-2.0f * x) + 2.0f, 2.0f) /
                                            2.0f);
}

inline f32 easeInCubic(f32 x) noexcept { return x * x * x; }

inline f32 easeOutCubic(f32 x) noexcept {
  return static_cast<f32>(1.0f - std::pow(1.0f - x, 3));
}

inline f32 easeInOutCubic(f32 x) noexcept {
  return x < 0.5f ? 4.0f * x * x * x
                  : 1.0f - static_cast<f32>(std::pow((-2.0f * x) + 2.0f, 3.0f) /
                                            2.0f);
}

inline f32 easeInQuart(f32 x) noexcept { return x * x * x * x; }

inline f32 easeOutQuart(f32 x) noexcept {
  return static_cast<f32>(1.0f - std::pow(1.0f - x, 4));
}

inline f32 easeInOutQuart(f32 x) noexcept {
  return x < 0.5f ? 8.0f * x * x * x * x
                  : 1.0f - static_cast<f32>(std::pow((-2.0f * x) + 2.0f, 4.0f) /
                                            2.0f);
}

inline f32 easeInQuint(f32 x) noexcept { return x * x * x * x * x; }

inline f32 easeOutQuint(f32 x) noexcept {
  return 1.0f - std::pow(1.0f - x, 5.0f);
}

inline f32 easeInOutQuint(f32 x) noexcept {
  return x < 0.5f ? 16.0f * x * x * x * x * x
                  : 1.0f - static_cast<f32>(std::pow((-2.0f * x) + 2.0f, 5.0f) /
                                            2.0f);
}

inline f32 easeInExpo(f32 x) noexcept {
  return x == 0.0f ? 0.0f : std::pow(2.0f, (10.0f * x) - 10.0f);
}

inline f32 easeOutExpo(f32 x) noexcept {
  return x == 1.0f ? 1.0f : 1.0f - static_cast<f32>(std::pow(2, -10 * x));
}

inline f32 easeInOutExpo(f32 x) noexcept {
  if (x == 0.0) {
    return 0.0;
  }
  if (x == 1.0) {
    return 1.0;
  }
  if (x < 0.5) {
    return static_cast<f32>(std::pow(2.0, (20.0 * x) - 10.0) / 2.0);
  }
  return (2.0f - std::pow(2.0f, (-20.0f * x) + 10.0f)) / 2.0f;
}

inline f32 easeInCirc(f32 x) noexcept {
  return 1.0f - std::sqrt(1.0f - std::pow(x, 2.0f));
}

inline f32 easeOutCirc(f32 x) noexcept {
  return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
}

inline f32 easeInOutCirc(f32 x) noexcept {
  return x < 0.5f
             ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * x, 2.0f))) / 2.0f
             : (std::sqrt(1.0f - std::pow((-2.0f * x) + 2.0f, 2.0f)) + 1.0f) /
                   2.0f;
}

inline f32 easeInBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158f;
  constexpr f32 c3 = c1 + 1.0f;
  return (c3 * x * x * x) - (c1 * x * x);
}

inline f32 easeOutBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158f;
  constexpr f32 c3 = c1 + 1.0f;
  return 1.0f + c3 + std::pow(x - 1.0f, 3.0f) + (c1 * std::pow(x - 1.0f, 2.0f));
}

inline f32 easeInOutBack(f32 x) noexcept {
  constexpr f32 c1 = 1.70158f;
  constexpr f32 c2 = c1 * 1.525f;
  return x < 0.5f
             ? (std::pow(2.0f * x, 2.0f) * (((c2 + 1.0f) * 2.0f * x) - c2)) /
                   2.0f
             : ((std::pow((2.0f * x) - 2.0f, 2.0f) *
                 (((c2 + 1.0f) * ((x * 2.0f) - 2.0f)) + c2)) +
                2.0f) /
                   2.0f;
}

inline f32 easeInElastic(f32 x) noexcept {
  constexpr f32 c4 = (2.0f * PI) / 3.0f;
  if (x == 0.0f) {
    return 0;
  }
  if (x == 1.0f) {
    return 1;
  }
  return std::sin(((x * 10.0f) - 10.75f) * c4);
}

inline f32 easeOutElastic(f32 x) noexcept {
  constexpr f32 c4 = (2.0f * PI) / 3.0f;
  if (x == 0.0f) {
    return 0;
  }
  if (x == 1.0f) {
    return 1;
  }
  return (std::pow(2.0f, -10.0f * x) * std::sin(((x * 10.0f) - 0.75f) * c4)) +
         1.0f;
}

inline f32 easeInOutElastic(f32 x) noexcept {
  constexpr f32 c5 = (2.0f * PI) / 4.5f;
  if (x == 0.0f) {
    return 0.0f;
  }
  if (x == 1.0f) {
    return 1.0f;
  }
  if (x < 0.5f) {
    return -(std::pow(2.0f, (20.0f * x) - 10.0f) *
             std::sin(((20.0f * x) - 11.125f) * c5)) /
           2.0f;
  }
  return ((std::pow(2.0f, (-20.0f * x) + 10.0f) *
           std::sin(((20.0f * x) - 11.125f) * c5)) /
          2.0f) +
         1.0f;
}

inline f32 easeOutBounce(f32 x) noexcept {
  const f32 kN1 = 7.5625f;
  const f32 kD1 = 2.75f;
  if (x < 1.0f / kD1) {
    return kN1 * x * x;
  }
  if (x < 2.0f / kD1) {
    x -= 1.5f / kD1;
    return (kN1 * x * x) + 0.75f;
  }
  if (x < 2.5f / kD1) {
    x -= 2.25f / kD1;
    return (kN1 * x * x) + 0.9375f;
  }
  x -= 2.625f / kD1;
  return (kN1 * x * x) + 0.984375f;
}

inline f32 easeInBounce(f32 x) noexcept {
  return 1.0f - easeOutBounce(1.0f - x);
}

inline f32 easeInOutBounce(f32 x) noexcept {
  return x < 0.5f ? (1.0f - easeOutBounce(1.0f - (2.0f * x))) / 2.0f
                  : (1.0f + easeOutBounce((2.0f * x) - 1.0f)) / 2.0f;
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_EASING_H_
