#include <uinta/math.hpp>

#include <algorithm> // std::min

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>

RunningAvg::RunningAvg(const unsigned int sample_size) noexcept {
    buffer = new float[sample_size];
    mavg = 0.0;
    dirty = false;
    cursor = 0u;
    count = sample_size;
}

RunningAvg::RunningAvg(const RunningAvg& other) noexcept {
    *this = other;
}

RunningAvg& RunningAvg::operator=(const RunningAvg &other) noexcept {
    buffer = other.buffer;
    mavg = other.mavg;
    dirty = other.dirty;
    cursor = other.cursor;
    count = other.count;
    return *this;
}

void RunningAvg::operator+=(const float v) noexcept {
    add(v);
}

RunningAvg::~RunningAvg() {
    delete[] buffer;
}

float RunningAvg::avg() noexcept {
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

void RunningAvg::add(float v) noexcept {
    buffer[cursor % count] = v;
    dirty = true;
    cursor++;
    // TODO check for cursor violating uint max
}

SmoothFloat::SmoothFloat(const float start, const float agility) noexcept : current(start), target(start), agility(agility) {
}

SmoothFloat::SmoothFloat(const SmoothFloat& other) noexcept {
    *this = other;
}

SmoothFloat& SmoothFloat::operator=(const SmoothFloat& other) noexcept {
    agility = other.agility;
    current = other.current;
    target = other.target;
    return *this;
}

SmoothFloat& SmoothFloat::operator=(const float v) noexcept {
    target = v;
    return *this;
}

SmoothFloat& SmoothFloat::operator+=(const float v) noexcept {
    target += v;
    return *this;
}

SmoothFloat& SmoothFloat::operator-=(const float v) noexcept {
    target -= v;
    return *this;
}

void SmoothFloat::force() noexcept {
    force(target);
}

void SmoothFloat::force(float v) noexcept {
    current = v;
    target = v;
}

void SmoothFloat::tick(const float dt) noexcept {
    float diff = (target - current);
    current += diff * agility * dt;
}

void updateViewMatrix(glm::mat4& view, const glm::vec3 pos, const float pitch, const float yaw) {
    glm::mat4 transform_x = glm::eulerAngleX(glm::radians(pitch));
    glm::mat4 transform_y = glm::eulerAngleY(glm::radians(yaw));
    view = glm::translate(transform_x * transform_y, -pos);
}
