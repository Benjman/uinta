#include <math.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>

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

#include <algorithm> // std::min
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

smooth_float::smooth_float(const float start, const float agility) noexcept : current(start), target(start), agility(agility) {
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

smooth_float& smooth_float::operator=(const float v) noexcept {
    target = v;
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

void smooth_float::force() noexcept {
    force(target);
}

void smooth_float::force(float v) noexcept {
    current = v;
    target = v;
}

#include <cstdio>
void smooth_float::tick(const float dt) noexcept {
    float diff = (target - current);
    current += diff * agility * dt;
}

void updateViewMatrix(glm::mat4& view, const glm::vec3 pos, const float pitch, const float yaw) {
    glm::mat4 transform_x = glm::eulerAngleX(glm::radians(pitch));
    glm::mat4 transform_y = glm::eulerAngleY(glm::radians(yaw));
    view = glm::translate(transform_x * transform_y, -pos);
}
