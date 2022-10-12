#ifndef UINTA_MATH_SMOOTH_FLOAT_HPP
#define UINTA_MATH_SMOOTH_FLOAT_HPP

namespace uinta {

struct SmoothFloat final {
  float agility;
  float current;
  float target;

  SmoothFloat(float agility, float start) noexcept;
  SmoothFloat(const SmoothFloat& other) noexcept;

  SmoothFloat& operator=(const SmoothFloat& other) noexcept;
  SmoothFloat& operator=(float v) noexcept;
  SmoothFloat& operator+=(float v) noexcept;
  SmoothFloat& operator-=(float v) noexcept;

  operator float() const;
};

// TODO these need ot be inlined
void force(SmoothFloat&);
void force(SmoothFloat&, float value);
void update(SmoothFloat&, float dt);

}  // namespace uinta

#endif  // UINTA_MATH_SMOOTH_FLOAT_HPP
