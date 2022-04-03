#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <math.hpp>

/** world coordinates follow the "right-handed rule" **/
const inline vec3 WORLD_UP      = vec3(0.0, 1.0,  0.0);
const inline vec3 WORLD_RIGHT   = vec3(1.0, 0.0,  0.0);
const inline vec3 WORLD_FORWARD = vec3(0.0, 0.0, -1.0);

struct camera2d final {
    vec2 pos = vec2(0.0);
    float fov = 45;
    float ortho_size = 1.0;
};

void get_view_matrix(const camera2d &camera, mat4 &mat) noexcept;
void get_ortho_matrix(mat4 &mat, float left, float right, float bottom, float top, float near, float far) noexcept;
void get_perspective_matrix(mat4 &mat, float fov, float width, float height, float near, float far) noexcept;

#endif // UINTA_CAMERA2D_H
