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
    float m01() { return values[1]; }
    float m02() { return values[2]; }
    float m03() { return values[3]; }
    float m10() { return values[4]; }
    float m11() { return values[5]; }
    float m12() { return values[6]; }
    float m13() { return values[7]; }
    float m20() { return values[8]; }
    float m21() { return values[9]; }
    float m22() { return values[10]; }
    float m23() { return values[11]; }
    float m30() { return values[12]; }
    float m31() { return values[13]; }
    float m32() { return values[14]; }
    float m33() { return values[15]; }

};

#endif // UINTA_MATH_H
