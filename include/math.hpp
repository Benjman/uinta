#ifndef UINTA_MATH_H
#define UINTA_MATH_H

#include <algorithm>
struct running_avg final {
public:
    unsigned int count;

    running_avg(const unsigned int) noexcept;
    running_avg(const running_avg&) noexcept;
    running_avg& operator=(const running_avg&) noexcept;

    void operator+=(const float) noexcept;

    ~running_avg();

    void add(float) noexcept;

    float avg() noexcept;

private:
    float* buffer;
    float mavg;
    bool dirty;
    unsigned int cursor;

};

#include <glm/vec3.hpp>
struct smooth_float final {
    float agility;
    float current;
    float target;

    smooth_float(const float start, const float agility) noexcept;
    smooth_float(const smooth_float& other) noexcept;

    smooth_float& operator=(const smooth_float& other) noexcept;
    smooth_float& operator=(const float v) noexcept;
    smooth_float& operator+=(const float v) noexcept;
    smooth_float& operator-=(const float v) noexcept;

    void force() noexcept;
    void force(float v) noexcept;

    void tick(const float dt) noexcept;

};

#include <glm/vec3.hpp>
struct smooth_vec3 {
    smooth_float values[3] {
        smooth_float(0.0, 5.0),
        smooth_float(0.0, 5.0),
        smooth_float(0.0, 5.0)
    };

    smooth_vec3(const smooth_float& x = smooth_float(0.0, 5.0), const smooth_float& y = smooth_float(0.0, 5.0), const smooth_float& z = smooth_float(0.0, 5.0)) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
    }

    smooth_vec3(const float x, const float y, const float z) {
        this->x(x);
        this->y(y);
        this->z(z);
    }

    smooth_vec3(const glm::vec3& v) : smooth_vec3(v.x, v.y, v.z) {}

    float x() { return smooth_float_x().current; }
    void x(float v) { smooth_float_x().target = v; }

    float y() { return smooth_float_y().current; }
    void y(float v) { smooth_float_y().target = v; }

    float z() { return smooth_float_z().current; }
    void z(float v) { smooth_float_z().target = v; }

    smooth_float& smooth_float_x() { return values[0]; }
    smooth_float& smooth_float_y() { return values[1]; }
    smooth_float& smooth_float_z() { return values[2]; }

    void tick(const float dt) {
        smooth_float_x().tick(dt);
        smooth_float_y().tick(dt);
        smooth_float_z().tick(dt);
    }

    operator glm::vec3() {
        return glm::vec3(x(), y(), z());
    }

    smooth_vec3& operator+=(const glm::vec3& v) {
        x(x() + v.x);
        y(y() + v.y);
        z(z() + v.z);
        return *this;
    }

};

inline float map0to1Range(const float value, const float min, const float max) {
    return (value - min) / (max - min);
}

inline float map0to1RangeClamped(const float value, const float min, const float max) {
    return std::clamp(map0to1Range(value, min, max), 0.0f, 1.0f);
}

#include <glm/mat4x4.hpp>
void updateViewMatrix(glm::mat4& view, const glm::vec3 pos, const float pitch, const float yaw);

#endif // UINTA_MATH_H
