#include <uinta/math/smooth_float.hpp>

SmoothFloat::SmoothFloat(const float start, const float agility) noexcept : current(start), target(start), agility(agility) {}

SmoothFloat::SmoothFloat(const SmoothFloat &other) noexcept { *this = other; }

SmoothFloat &SmoothFloat::operator=(const SmoothFloat &other) noexcept {
  agility = other.agility;
  current = other.current;
  target  = other.target;
  return *this;
}

SmoothFloat &SmoothFloat::operator=(const float v) noexcept {
  target = v;
  return *this;
}

SmoothFloat &SmoothFloat::operator+=(const float v) noexcept {
  target += v;
  return *this;
}

SmoothFloat &SmoothFloat::operator-=(const float v) noexcept {
  target -= v;
  return *this;
}

void SmoothFloat::force() noexcept { force(target); }

void SmoothFloat::force(float v) noexcept {
  current = v;
  target  = v;
}

void SmoothFloat::tick(const float dt) noexcept {
  float diff = (target - current);
  current += diff * agility * dt;
}

