#ifndef UINTA_VEC_H
#define UINTA_VEC_H

struct vec2 final {
    float x;
    float y;

    vec2() noexcept : vec2(0.0) {}

    vec2(float v) noexcept : vec2(v, v) {}

    vec2(float x, float y) noexcept {
        this->x = x;
        this->y = y;
    }

    vec2(const vec2 &vec) noexcept {
        *this = vec;
    }

    vec2& operator=(const vec2 &rhs) noexcept {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    vec2 operator+(const vec2 &rhs) const noexcept {
        return vec2(x + rhs.x, y + rhs.y);
    }

    vec2 operator-(const vec2 &rhs) const noexcept {
        return vec2(x - rhs.x, y - rhs.y);
    }

    bool operator>(const vec2 &rhs) const noexcept {
        return x > rhs.x && y > rhs.y;
    }

    bool operator<(const vec2 &rhs) const noexcept {
        return x < rhs.x && y < rhs.y;
    }

    bool operator>=(const vec2 &rhs) const noexcept {
        return x >= rhs.x && y >= rhs.y;
    }

    bool operator<=(const vec2 &rhs) const noexcept {
        return x <= rhs.x && y <= rhs.y;
    }

    bool operator==(const vec2 &rhs) const noexcept {
        return x == rhs.x && y == rhs.y;
    }

};

struct vec3 final {
    float x;
    float y;
    float z;

    vec3() noexcept : vec3(0.0) {}

    vec3(float v) noexcept : vec3(v, v, v) {}

    vec3(float x, float y, float z) noexcept {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3(const vec3& vec) noexcept {
        *this = vec;
    }

    vec3& operator=(const vec3& vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    vec3 operator+(const vec3 &other) noexcept {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3 &other) noexcept {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    bool operator>(const vec3 &rhs) const noexcept {
        return x > rhs.x && y > rhs.y && z > rhs.z;
    }

    bool operator<(const vec3 &rhs) const noexcept {
        return x < rhs.x && y < rhs.y && z < rhs.z;
    }

    bool operator>=(const vec3 &rhs) const noexcept {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    bool operator<=(const vec3 &rhs) const noexcept {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    bool operator==(const vec3 &rhs) const noexcept {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

};

#endif // UINTA_VEC_H
