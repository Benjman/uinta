#ifndef UINTA_MATH_H
#define UINTA_MATH_H

struct vec2 final {
    float x;
    float y;

    vec2() noexcept;
    vec2(float v) noexcept;
    vec2(float x, float y) noexcept;
    vec2(const vec2 &vec) noexcept;

    vec2& operator=(const vec2 &rhs) noexcept;
    vec2 operator*(const vec2 &rhs) const noexcept;
    vec2 operator*(float v) const noexcept;
    vec2 operator*=(const vec2 &rhs) noexcept;
    vec2 operator+(const vec2 &rhs) const noexcept;
    vec2 operator-(const vec2 &rhs) const noexcept;
    vec2 operator/(const vec2 &rhs) noexcept;
    vec2 operator/=(const vec2 &rhs) noexcept;

    bool operator!=(const vec2 &rhs) const noexcept;
    bool operator<(const vec2 &rhs) const noexcept;
    bool operator<=(const vec2 &rhs) const noexcept;
    bool operator==(const vec2 &rhs) const noexcept;
    bool operator>(const vec2 &rhs) const noexcept;
    bool operator>=(const vec2 &rhs) const noexcept;

};

struct vec3 final {
    float x;
    float y;
    float z;

    vec3() noexcept;
    vec3(float v) noexcept;
    vec3(float x, float y, float z) noexcept;
    vec3(const vec3& vec) noexcept;
    vec3(const vec2& vec) noexcept;
    vec3(const vec2& vec, float z) noexcept;

    vec3& operator=(const vec3& vec) noexcept;
    vec3 operator*(const vec3 &rhs) const noexcept;
    vec3 operator*(float v) const noexcept;
    vec3 operator*=(const vec3 &rhs) noexcept;
    vec3 operator+(const vec3 &other) noexcept;
    vec3 operator-(const vec3 &other) noexcept;
    vec3 operator/(const vec3 &rhs) noexcept;
    vec3 operator/=(const vec3 &rhs) noexcept;

    bool operator!=(const vec3 &rhs) const noexcept;
    bool operator<(const vec3 &rhs) const noexcept;
    bool operator<=(const vec3 &rhs) const noexcept;
    bool operator==(const vec3 &rhs) const noexcept;
    bool operator>(const vec3 &rhs) const noexcept;
    bool operator>=(const vec3 &rhs) const noexcept;

};

struct vec4 final {
    float x;
    float y;
    float z;
    float w;

    vec4() noexcept : vec4(0.0) {}
    vec4(float v) noexcept : vec4(v, v, v, v) {}
    vec4(float x, float y, float z, float w) noexcept;
    vec4(const vec4& vec) noexcept;
    vec4(const vec2& vec) noexcept;
    vec4(const vec2& vec, float z) noexcept;
    vec4(const vec2& vec, float z, float w) noexcept;
    vec4(const vec3& vec) noexcept;
    vec4(const vec3& vec, float w) noexcept;

    vec4& operator=(const vec4& vec) noexcept;
    vec4 operator*(const vec4 &rhs) const noexcept;
    vec4 operator*(float v) const noexcept;
    vec4 operator*=(const vec4 &rhs) noexcept;
    vec4 operator+(const vec4 &other) noexcept;
    vec4 operator-(const vec4 &other) noexcept;
    vec4 operator/(const vec4 &rhs) noexcept;
    vec4 operator/=(const vec4 &rhs) noexcept;

    bool operator!=(const vec4 &rhs) const noexcept;
    bool operator<(const vec4 &rhs) const noexcept;
    bool operator<=(const vec4 &rhs) const noexcept;
    bool operator==(const vec4 &rhs) const noexcept;
    bool operator>(const vec4 &rhs) const noexcept;
    bool operator>=(const vec4 &rhs) const noexcept;

};

/** matricies are column major **/
struct mat4 final {
    float values[16];

    mat4();

    mat4(float v) noexcept;
    mat4(const float *values) noexcept;
    mat4(const mat4 &other) noexcept;
    mat4(const vec4 &other) noexcept;

    mat4& operator=(const mat4 &rhs) noexcept;

    mat4& operator*(const vec4 &rhs) noexcept;
    mat4& operator*=(const vec4 &rhs) noexcept;
    mat4& operator=(const vec4 &rhs) noexcept;

    float m00() { return values[0]; }
    void m00(float v) { values[0] = v; }
    float m01() { return values[1]; }
    void m01(float v) { values[1] = v; }
    float m02() { return values[2]; }
    void m02(float v) { values[2] = v; }
    float m03() { return values[3]; }
    void m03(float v) { values[3] = v; }
    float m10() { return values[4]; }
    void m10(float v) { values[4] = v; }
    float m11() { return values[5]; }
    void m11(float v) { values[5] = v; }
    float m12() { return values[6]; }
    void m12(float v) { values[6] = v; }
    float m13() { return values[7]; }
    void m13(float v) { values[7] = v; }
    float m20() { return values[8]; }
    void m20(float v) { values[8] = v; }
    float m21() { return values[9]; }
    void m21(float v) { values[9] = v; }
    float m22() { return values[10]; }
    void m22(float v) { values[10] = v; }
    float m23() { return values[11]; }
    void m23(float v) { values[11] = v; }
    float m30() { return values[12]; }
    void m30(float v) { values[12] = v; }
    float m31() { return values[13]; }
    void m31(float v) { values[13] = v; }
    float m32() { return values[14]; }
    void m32(float v) { values[14] = v; }
    float m33() { return values[15]; }
    void m33(float v) { values[15] = v; }

};

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

struct smooth_float final {
    float agility;
    float current;
    float target;

    smooth_float(const float agility = 1.0, const float target = 0.0) noexcept;
    smooth_float(const smooth_float& other) noexcept;

    smooth_float& operator=(const smooth_float& other) noexcept;

    smooth_float& operator+=(const float v) noexcept;

    smooth_float& operator-=(const float v) noexcept;

    operator float() const noexcept;

    void force() noexcept;
    void force(float v) noexcept;

    void tick(const float dt) noexcept;

};

#endif // UINTA_MATH_H
