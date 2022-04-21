#include <camera.hpp>
#include <cstdio>
#include <cstring> // for memcpy

// TODO get rid of glm -- but need to learn quaternions tho :'(
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

void get_view_matrix(const camera2d& camera, mat4* mat) noexcept {
    *mat = mat4();
    mat->m00(WORLD_RIGHT.x);
    mat->m01(WORLD_RIGHT.y);
    mat->m02(WORLD_RIGHT.z);
    mat->m10(WORLD_UP.x);
    mat->m11(WORLD_UP.y);
    mat->m12(WORLD_UP.z);
    mat->m20(WORLD_FORWARD.x);
    mat->m21(WORLD_FORWARD.y);
    mat->m22(WORLD_FORWARD.z);
    mat->m30(camera.pos.x);
    mat->m31(camera.pos.y);

    // TODO get rid of glm and do our own 4x4 matrix inverse
    glm::mat4 gmat = glm::mat4(*mat->values);
    memcpy(&gmat[0][0], mat, sizeof(float) * 16);
    gmat = glm::inverse(gmat);
    memcpy(mat, &gmat[0][0], sizeof(float) * 16);
}

void get_view_matrix(const camera3d& camera, glm::mat4* mat) noexcept {
    *mat = glm::rotate(glm::mat4(1.0), glm::radians(camera.attitude.x), WORLD_RIGHT);
    *mat = glm::rotate(*mat, glm::radians(camera.attitude.y), WORLD_UP);
    *mat = glm::translate(*mat, camera.pos);
    *mat = glm::inverse(*mat);
}

void get_ortho_matrix(mat4 &mat, float left, float right, float bottom, float top, float near, float far) noexcept {
    glm::mat4 ortho = glm::ortho(left, right, bottom, top, near, far);
    memcpy(&mat, &ortho, sizeof(float) * 16);
}

void get_perspective_matrix(mat4 &mat, float fov, float width, float height, float near, float far) noexcept {
    glm::mat4 proj = glm::perspective(glm::radians(fov), width / height, near, far);
    memcpy(mat.values, &proj, sizeof(float) * 16);
}

