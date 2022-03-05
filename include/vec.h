#ifndef UINTA_POS_H
#define UINTA_POS_H

struct vec2d final {
    float x = 0.0;
    float y = 0.0;

    vec2d() = default;

    vec2d(float x, float y) {
        this->x = x;
        this->y = y;
    }

    vec2d(const vec2d &vec) {
        *this = vec;
    }

    vec2d& operator=(const vec2d &vec) {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    vec2d operator+(const vec2d &other) {
        return vec2d(x + other.x, y + other.y);
    }

    vec2d operator-(const vec2d &other) {
        return vec2d(x - other.x, y - other.y);
    }

    bool operator>(const vec2d &rhs) const {
        return x > rhs.x && y > rhs.y;
    }

    bool operator<(const vec2d &rhs) const {
        return x < rhs.x && y < rhs.y;
    }

    bool operator>=(const vec2d &rhs) const {
        return x >= rhs.x && y >= rhs.y;
    }

    bool operator<=(const vec2d &rhs) const {
        return x <= rhs.x && y <= rhs.y;
    }

    bool operator==(const vec2d &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

};

struct vec3d final {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    vec3d() = default;

    vec3d(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3d(const vec3d& vec) {
        *this = vec;
    }

    vec3d& operator=(const vec3d& vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    vec3d operator+(const vec3d &other) {
        return vec3d(x + other.x, y + other.y, z + other.z);
    }

    vec3d operator-(const vec3d &other) {
        return vec3d(x - other.x, y - other.y, z - other.z);
    }

    bool operator>(const vec3d &rhs) const {
        return x > rhs.x && y > rhs.y && z > rhs.z;
    }

    bool operator<(const vec3d &rhs) const {
        return x < rhs.x && y < rhs.y && z < rhs.z;
    }

    bool operator>=(const vec3d &rhs) const {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    bool operator<=(const vec3d &rhs) const {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    bool operator==(const vec3d &rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

};

#endif // UINTA_vec_H
