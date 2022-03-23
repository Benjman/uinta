#include <camera2d.hpp>
#include <cstring> // for memcpy

// TODO get rid of glm -- but need to learn quaternions tho :'(
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

void get_view_matrix(const camera2d &camera, mat4 &mat) noexcept {
    glm::vec3 pos = glm::vec3(camera.pos.x, camera.pos.y, 0.0);
    glm::vec3 forward = glm::vec3(WORLD_FORWARD.x, WORLD_FORWARD.y, WORLD_FORWARD.z);
    glm::vec3 up = glm::vec3(WORLD_UP.x, WORLD_UP.y, WORLD_UP.z);
    glm::mat4 view = glm::lookAt(pos, pos + forward, up);
    memcpy(&mat, &view, sizeof(float) * 16);
}

void get_ortho_matrix(mat4 &mat, float left, float right, float bottom, float top, float near, float far) noexcept {
    // mat = mat4(1.0);
    // mat.values[0] = 2 / (right - left);
    // mat.values[5] = 2 / (top - bottom);
    // mat.values[10] = 2 / (far - near);
    // mat.values[12] = -(right+left) / (right-left);
    // mat.values[13] = -(top+bottom) / (top-bottom);
    // mat.values[14] = -(far+near) / (far-near);
    glm::mat4 ortho = glm::ortho(left, right, bottom, top);
    memcpy(&mat, &ortho, sizeof(float) * 16);
}

void get_perspective_matrix(mat4 &mat, float fov, float width, float height, float near, float far) noexcept {
    glm::mat4 proj = glm::perspective(glm::radians(fov), width / height, near, far);
    memcpy(mat.values, &proj, sizeof(float) * 16);
}

