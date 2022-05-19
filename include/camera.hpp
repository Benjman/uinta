#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <math.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/** world coordinates follow the "right-handed rule" **/
const inline glm::vec3 WORLD_UP      = glm::vec3(0.0, 1.0,  0.0);
const inline glm::vec3 WORLD_RIGHT   = glm::vec3(1.0, 0.0,  0.0);
const inline glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 0.0, -1.0);

struct camera final {
    glm::vec3 pos = glm::vec3(0.0);
    glm::vec3 attitude = glm::vec3(0.0);

    inline float pitch() const noexcept { return attitude.x; }
    inline float yaw() const noexcept { return attitude.y; }
    inline float roll() const noexcept { return attitude.z; }

    inline void pitch(const float v) noexcept {
        if (v == attitude.x)
            return;
        attitude.x = v;
    }

    inline void yaw(const float v) noexcept {
        if (v == attitude.y)
            return;
        attitude.y = v;
    }

    inline void roll(const float v) noexcept {
        if (v == attitude.z)
            return;
        attitude.z = v;
    }

};


struct camera2d final {
    vec2 pos = vec2(0.0);
    float fov = 45;
    float ortho_size = 1.0;
};

void get_view_matrix(glm::mat4* mat, const glm::vec3& pos, const float pitch, const float yaw) noexcept;

#endif // UINTA_CAMERA2D_H
