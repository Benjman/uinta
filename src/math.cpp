#include <math.hpp>

vec2::vec2() noexcept : vec2(0.0) {}

vec2::vec2(float v) noexcept : vec2(v, v) {}

vec2::vec2(float x, float y) noexcept {
    this->x = x;
    this->y = y;
}

vec2::vec2(const vec2 &vec) noexcept {
    *this = vec;
}

vec2& vec2::operator=(const vec2 &rhs) noexcept {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

vec2 vec2::operator+(const vec2 &rhs) const noexcept {
    return vec2(x + rhs.x, y + rhs.y);
}

vec2 vec2::operator-(const vec2 &rhs) const noexcept {
    return vec2(x - rhs.x, y - rhs.y);
}

vec2 vec2::operator*(const vec2 &rhs) const noexcept {
    return vec2(x * rhs.x, y * rhs.y);
}

vec2 vec2::operator*(float v) const noexcept {
    return vec2(x * v, y * v);
}

vec2 vec2::operator*=(const vec2 &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

vec2 vec2::operator/(const vec2 &rhs) noexcept {
    return vec2(x / rhs.x, y / rhs.y);
}

vec2 vec2::operator/=(const vec2 &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

bool vec2::operator>(const vec2 &rhs) const noexcept {
    return x > rhs.x && y > rhs.y;
}

bool vec2::operator<(const vec2 &rhs) const noexcept {
    return x < rhs.x && y < rhs.y;
}

bool vec2::operator>=(const vec2 &rhs) const noexcept {
    return x >= rhs.x && y >= rhs.y;
}

bool vec2::operator<=(const vec2 &rhs) const noexcept {
    return x <= rhs.x && y <= rhs.y;
}

bool vec2::operator==(const vec2 &rhs) const noexcept {
    return x == rhs.x && y == rhs.y;
}

bool vec2::operator!=(const vec2 &rhs) const noexcept {
    return x != rhs.x || y != rhs.y;
}

vec3::vec3() noexcept : vec3(0.0) {}

vec3::vec3(float v) noexcept : vec3(v, v, v) {}

vec3::vec3(float x, float y, float z) noexcept {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3::vec3(const vec3& vec) noexcept {
    *this = vec;
}

vec3::vec3(const vec2& vec) noexcept : vec3(vec, 0.0) {}

vec3::vec3(const vec2& vec, float z) noexcept : vec3(vec.x, vec.y, z) {}

vec3& vec3::operator=(const vec3& vec) noexcept {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

vec3 vec3::operator+(const vec3 &other) noexcept {
    return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3 &other) noexcept {
    return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator*(const vec3 &rhs) const noexcept {
    return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
}

vec3 vec3::operator*(float v) const noexcept {
    return vec3(x * v, y * v, z * v);
}

vec3 vec3::operator*=(const vec3 &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

vec3 vec3::operator/(const vec3 &rhs) noexcept {
    return vec3(x / rhs.x, y / rhs.y, z / rhs.z);
}

vec3 vec3::operator/=(const vec3 &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

bool vec3::operator>(const vec3 &rhs) const noexcept {
    return x > rhs.x && y > rhs.y && z > rhs.z;
}

bool vec3::operator<(const vec3 &rhs) const noexcept {
    return x < rhs.x && y < rhs.y && z < rhs.z;
}

bool vec3::operator>=(const vec3 &rhs) const noexcept {
    return x >= rhs.x && y >= rhs.y && z >= rhs.z;
}

bool vec3::operator<=(const vec3 &rhs) const noexcept {
    return x <= rhs.x && y <= rhs.y && z <= rhs.z;
}

bool vec3::operator==(const vec3 &rhs) const noexcept {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool vec3::operator!=(const vec3 &rhs) const noexcept {
    return x != rhs.x || y != rhs.y || z == rhs.z;
}

vec4::vec4(float x, float y, float z, float w) noexcept {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

vec4::vec4(const vec4& vec) noexcept {
    *this = vec;
}

vec4::vec4(const vec2& vec) noexcept : vec4(vec, 0.0, 0.0) {}

vec4::vec4(const vec2& vec, float z) noexcept : vec4(vec.x, vec.y, z, 0.0) {}

vec4::vec4(const vec2& vec, float z, float w) noexcept : vec4(vec.x, vec.y, z, w) {}

vec4::vec4(const vec3& vec) noexcept : vec4(vec, 0.0) {}

vec4::vec4(const vec3& vec, float w) noexcept : vec4(vec.x, vec.y, vec.z, w) {}

vec4& vec4::operator=(const vec4& vec) noexcept {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
    return *this;
}

vec4 vec4::operator+(const vec4 &other) noexcept {
    return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

vec4 vec4::operator-(const vec4 &other) noexcept {
    return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

vec4 vec4::operator*(const vec4 &rhs) const noexcept {
    return vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

vec4 vec4::operator*(float v) const noexcept {
    return vec4(x * v, y * v, z * v, w * v);
}

vec4 vec4::operator*=(const vec4 &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
}

vec4 vec4::operator/(const vec4 &rhs) noexcept {
    return vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

vec4 vec4::operator/=(const vec4 &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
}

bool vec4::operator>(const vec4 &rhs) const noexcept {
    return x > rhs.x && y > rhs.y && z > rhs.z && w > rhs.w;
}

bool vec4::operator<(const vec4 &rhs) const noexcept {
    return x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w;
}

bool vec4::operator>=(const vec4 &rhs) const noexcept {
    return x >= rhs.x && y >= rhs.y && z >= rhs.z && w >= rhs.w;
}

bool vec4::operator<=(const vec4 &rhs) const noexcept {
    return x <= rhs.x && y <= rhs.y && z <= rhs.z && w <= rhs.w;
}

bool vec4::operator==(const vec4 &rhs) const noexcept {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool vec4::operator!=(const vec4 &rhs) const noexcept {
    return x != rhs.x || y != rhs.y || z == rhs.z && w != rhs.w;
}

#include <cstring> // for memcpy
mat4::mat4(const float *values) noexcept {
    /*
       FIXME memcpy of matrices with different dimensions will dirty our array.
             For example, a mat3 has 12 floats, but if it's sent to this constructor 
             it will copy 16 floats, filling the tail of our array with garbage.
    */
    memcpy(this->values, values, sizeof(float) * 16);
}

mat4::mat4() {
    values[0]  = 1.0;
    values[1]  = 0.0;
    values[2]  = 0.0;
    values[3]  = 0.0;
    values[4]  = 0.0;
    values[5]  = 1.0;
    values[6]  = 0.0;
    values[7]  = 0.0;
    values[8]  = 0.0;
    values[9]  = 0.0;
    values[10] = 1.0;
    values[11] = 0.0;
    values[12] = 0.0;
    values[13] = 0.0;
    values[14] = 0.0;
    values[15] = 1.0;
}

mat4::mat4(float v) noexcept : mat4() {
    values[0] = v;
    values[5] = v;
    values[10] = v;
    values[15] = v;
}

mat4::mat4(const mat4 &other) noexcept {
    *this = mat4(other.values);
}

mat4& mat4::operator=(const mat4 &rhs) noexcept {
    memcpy(this->values, rhs.values, sizeof(float) * 16);
    return *this;
}

mat4::mat4(const vec4 &other) noexcept : mat4() {
    values[0] = other.x;
    values[5] = other.y;
    values[10] = other.z;
    values[15] = other.w;
}

mat4& mat4::operator=(const vec4 &rhs) noexcept {
    *this = mat4();
    values[0] = rhs.x;
    values[5] = rhs.y;
    values[10] = rhs.z;
    values[15] = rhs.w;
    return *this;
}

mat4& mat4::operator*(const vec4 &rhs) noexcept {
    float tmp[16];
    memcpy(tmp, values, sizeof(tmp));
    tmp[0]  *= rhs.x;
    tmp[1]  *= rhs.x;
    tmp[2]  *= rhs.x;
    tmp[3]  *= rhs.x;
    tmp[4]  *= rhs.y;
    tmp[5]  *= rhs.y;
    tmp[6]  *= rhs.y;
    tmp[7]  *= rhs.y;
    tmp[8]  *= rhs.z;
    tmp[9]  *= rhs.z;
    tmp[10] *= rhs.z;
    tmp[11] *= rhs.z;
    tmp[12] *= rhs.w;
    tmp[13] *= rhs.w;
    tmp[14] *= rhs.w;
    tmp[15] *= rhs.w;
    return *this;
}

mat4& mat4::operator*=(const vec4 &rhs) noexcept {
    values[0] *= rhs.x;
    values[1] *= rhs.x;
    values[2] *= rhs.x;
    values[3] *= rhs.x;
    values[4] *= rhs.y;
    values[5] *= rhs.y;
    values[6] *= rhs.y;
    values[7] *= rhs.y;
    values[8] *= rhs.z;
    values[9] *= rhs.z;
    values[10] *= rhs.z;
    values[11] *= rhs.z;
    values[12] *= rhs.w;
    values[13] *= rhs.w;
    values[14] *= rhs.w;
    values[15] *= rhs.w;
    return *this;
}

running_avg::running_avg(const unsigned int sample_size) noexcept {
    buffer = new float[sample_size];
    mavg = 0.0;
    dirty = false;
    cursor = 0u;
    count = sample_size;
}

running_avg::running_avg(const running_avg& other) noexcept {
    *this = other;
}

running_avg& running_avg::operator=(const running_avg &other) noexcept {
    buffer = other.buffer;
    mavg = other.mavg;
    dirty = other.dirty;
    cursor = other.cursor;
    count = other.count;
    return *this;
}

void running_avg::operator+=(const float v) noexcept {
    add(v);
}

running_avg::~running_avg() {
    delete[] buffer;
}

float running_avg::avg() noexcept {
    if (!cursor)
        return 0.0;
    if (dirty) {
        float sum = 0.0;
        unsigned int len = std::min(cursor, count);
        for (unsigned int i = 0u; i < len; i++) {
            sum += buffer[i];
        }
        mavg = sum / (float) len;
        dirty = false;
    }
    return mavg;
}

void running_avg::add(float v) noexcept {
    buffer[cursor % count] = v;
    dirty = true;
    cursor++;
    // TODO check for cursor violating uint max
}

smooth_float::smooth_float(const float agility, const float target) noexcept : agility(agility), current(target), target(target) {
}

smooth_float::smooth_float(const smooth_float& other) noexcept {
    *this = other;
}

smooth_float& smooth_float::operator=(const smooth_float& other) noexcept {
    agility = other.agility;
    current = other.current;
    target = other.target;
    return *this;
}

smooth_float& smooth_float::operator+=(const float v) noexcept {
    target += v;
    return *this;
}

smooth_float& smooth_float::operator-=(const float v) noexcept {
    target -= v;
    return *this;
}

smooth_float::operator float() const noexcept {
    return current;
}

void smooth_float::force() noexcept {
    force(target);
}

void smooth_float::force(float v) noexcept {
    current = v;
    target = v;
}

void smooth_float::update(const float dt) noexcept {
    float diff = (target - current);
    current += diff * agility * dt;
}
