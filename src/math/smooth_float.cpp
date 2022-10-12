#include <uinta/math/smooth_float.hpp>

uinta::SmoothFloat::SmoothFloat(float agility, float start) noexcept : agility(agility), current(start), target(start) {
}

uinta::SmoothFloat::SmoothFloat(const SmoothFloat& other) noexcept {
  *this = other;
}

uinta::SmoothFloat& uinta::SmoothFloat::operator=(const SmoothFloat& other) noexcept {
  agility = other.agility;
  current = other.current;
  target = other.target;
  return *this;
}

uinta::SmoothFloat& uinta::SmoothFloat::operator=(float v) noexcept {
  target = v;
  return *this;
}

uinta::SmoothFloat& uinta::SmoothFloat::operator+=(float v) noexcept {
  target += v;
  return *this;
}

uinta::SmoothFloat& uinta::SmoothFloat::operator-=(float v) noexcept {
  target -= v;
  return *this;
}

uinta::SmoothFloat::operator float() const {
  return current;
}

void uinta::force(uinta::SmoothFloat& v) {
  force(v, v.target);
}

void uinta::force(uinta::SmoothFloat& v, float value) {
  v.current = value;
  v.target = value;
}

void uinta::update(uinta::SmoothFloat& v, float dt) {
  auto diff = (v.target - v.current);
  v.current += diff * v.agility * dt;
}
