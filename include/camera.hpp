#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <math.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/** world coordinates follow the "right-handed rule" **/
const inline glm::vec3 WORLD_UP      = glm::vec3(0.0, 1.0,  0.0);
const inline glm::vec3 WORLD_RIGHT   = glm::vec3(1.0, 0.0,  0.0);
const inline glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 0.0, -1.0);

struct camera2d final {
    vec2 pos = vec2(0.0);
    float fov = 45;
    float ortho_size = 1.0;
};

struct camera3d final {
    glm::vec3 pos;

    // attitude encompases the pitch, yaw, and roll. x, y, z respectively
    glm::vec3 attitude;

    float fov = 45;
    float ortho_size = 1.0;
};

struct camera3d_smooth final {
    smooth_float pos_x = smooth_float(5.0);
    smooth_float pos_y = smooth_float(5.0);
    smooth_float pos_z = smooth_float(5.0);

    // attitude describes the pitch (x), yaw (y), and roll (z)
    glm::vec3 attitude;

    float fov = 45;

    camera3d_smooth() noexcept : camera3d_smooth(1.0) {
    }

    camera3d_smooth(float agility) noexcept :
        pos_x(agility), pos_y(agility), pos_z(agility) {
    }
};


void get_view_matrix(const camera2d& camera, mat4* mat) noexcept;
void get_view_matrix(const glm::vec3& pos, const glm::vec3 attitude, glm::mat4* mat) noexcept;

void get_ortho_matrix(mat4& mat, float left, float right, float bottom, float top, float near, float far) noexcept;

void get_perspective_matrix(mat4& mat, float fov, float width, float height, float near, float far) noexcept;

#endif // UINTA_CAMERA2D_H
