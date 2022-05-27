#include <camera.hpp>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
void camera::update_view_matrix() {
    glm::mat4 transformx = glm::eulerAngleX(glm::radians(attitude.x));
    glm::mat4 transformy = glm::eulerAngleY(glm::radians(attitude.y));
    glm::mat4 transformz = glm::eulerAngleZ(glm::radians(attitude.z));
    m_view_matrix = glm::inverse(glm::translate(transformx * transformy * transformz, pos));
}
