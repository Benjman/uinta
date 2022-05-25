#include "camera_controller.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

void camera_controller::view_matrix(glm::mat4* const mat) {
    glm::vec3 pos(target_x.current, target_y.current, target_z.current);
    glm::mat4 transformx = glm::eulerAngleX(glm::radians(target_pitch.current));
    glm::mat4 transformy = glm::eulerAngleY(glm::radians(target_yaw.current));
    glm::mat4 transformz = glm::eulerAngleZ(glm::radians(target_roll.current));
    *mat = glm::inverse(glm::translate(transformx * transformy * transformz, pos));
}
