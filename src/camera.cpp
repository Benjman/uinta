#include <camera.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

void get_view_matrix(glm::mat4* mat, const glm::vec3& pos, const float pitch, const float yaw) noexcept {
    glm::mat4 transformx = glm::eulerAngleX(glm::radians(pitch));
    glm::mat4 transformy = glm::eulerAngleY(glm::radians(yaw));
    *mat = glm::translate(transformx * transformy, -pos);
}

