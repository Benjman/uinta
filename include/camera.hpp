#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <math.hpp>

#include <glm/mat4x4.hpp>

/** world coordinates follow the "right-handed rule" **/
const inline glm::vec3 WORLD_UP      = glm::vec3(0.0, 1.0,  0.0);
const inline glm::vec3 WORLD_RIGHT   = glm::vec3(1.0, 0.0,  0.0);
const inline glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 0.0, -1.0);

struct camera {
    glm::vec3 pos = glm::vec3(0.0);
    glm::vec3 attitude = glm::vec3(0.0);
    glm::mat4 m_view_matrix = glm::mat4(1.0);

    inline float pitch() const noexcept { return attitude.x; }
    inline void pitch(const float v) noexcept {
        if (v == attitude.x)
            return;
        attitude.x = v;
    }

    inline float yaw() const noexcept { return attitude.y; }
    inline void yaw(const float v) noexcept {
        if (v == attitude.y)
            return;
        attitude.y = v;
    }

    inline float roll() const noexcept { return attitude.z; }
    inline void roll(const float v) noexcept {
        if (v == attitude.z)
            return;
        attitude.z = v;
    }

    void update_view_matrix();

};


struct target_cam : camera {
    smooth_vec3 target;

    float dist = 10.0;
    float angle = 0.0;

    void tick(const float dt) {
        target.tick(dt);
        pos = glm::vec3(target.x(), target.y(), target.z());
    }

};

#endif // UINTA_CAMERA2D_H
