#ifndef UINTA_MATH_H
#define UINTA_MATH_H

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

    // TODO math operators against glm::vec3
};

#endif // UINTA_MATH_H
