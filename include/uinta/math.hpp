#ifndef UINTA_MATH_H
#define UINTA_MATH_H

#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace uinta {

struct RunningAvg final {
public:
  unsigned int count;

  RunningAvg(const unsigned int) noexcept;
  RunningAvg(const RunningAvg &) noexcept;
  RunningAvg &operator=(const RunningAvg &) noexcept;

  void operator+=(const float) noexcept;

  ~RunningAvg();

  void add(float) noexcept;

  float avg() noexcept;

private:
  float *buffer;
  float mavg;
  bool dirty;
  unsigned int cursor;
};

struct SmoothFloat final {
  float agility;
  float current;
  float target;

  SmoothFloat(const float start, const float agility) noexcept;
  SmoothFloat(const SmoothFloat &other) noexcept;

  SmoothFloat &operator=(const SmoothFloat &other) noexcept;
  SmoothFloat &operator=(const float v) noexcept;
  SmoothFloat &operator+=(const float v) noexcept;
  SmoothFloat &operator-=(const float v) noexcept;

  void force() noexcept;
  void force(float v) noexcept;

  void tick(const float dt) noexcept;
};

struct SmoothVec3 {
  SmoothFloat values[3]{SmoothFloat(0.0, 10.0), SmoothFloat(0.0, 10.0), SmoothFloat(0.0, 10.0)};

  SmoothVec3(const SmoothFloat &x = SmoothFloat(0.0, 10.0), const SmoothFloat &y = SmoothFloat(0.0, 10.0),
             const SmoothFloat &z = SmoothFloat(0.0, 10.0)) {
    smooth_float_x() = x;
    smooth_float_y() = y;
    smooth_float_z() = z;
  }

  SmoothVec3(const float x, const float y, const float z) {
    this->x(x);
    this->y(y);
    this->z(z);
  }

  SmoothVec3(const glm::vec3 &v) : SmoothVec3(v.x, v.y, v.z) {}

  SmoothFloat &smooth_float_x() { return values[0]; }
  SmoothFloat &smooth_float_y() { return values[1]; }
  SmoothFloat &smooth_float_z() { return values[2]; }

  float x() { return smooth_float_x().current; }
  void x(float v) { smooth_float_x().target = v; }

  float y() { return smooth_float_y().current; }
  void y(float v) { smooth_float_y().target = v; }

  float z() { return smooth_float_z().current; }
  void z(float v) { smooth_float_z().target = v; }

  void tick(const float dt) {
    smooth_float_x().tick(dt);
    smooth_float_y().tick(dt);
    smooth_float_z().tick(dt);
  }

  operator glm::vec3() { return glm::vec3(x(), y(), z()); }

  SmoothVec3 &operator+=(const glm::vec3 &v) {
    smooth_float_x() += v.x;
    smooth_float_y() += v.y;
    smooth_float_z() += v.z;
    return *this;
  }
};

void updateViewMatrix(glm::mat4 &view, const glm::vec3 pos, const float pitch, const float yaw);

inline float map0to1Range(const float value, const float min, const float max) { return (value - min) / (max - min); }

inline float map0to1RangeClamped(const float value, const float min, const float max) {
  return std::clamp(map0to1Range(value, min, max), 0.0f, 1.0f);
}

} // namespace uinta

#endif // UINTA_MATH_H
