#include <algorithm>  // std::min, std::clamp
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <uinta/math.hpp>

#include "./math/running_avg.cpp"
#include "./math/smooth_float.cpp"

using namespace uinta;

void uinta::genViewMatrix(glm::mat4& view, const glm::vec3& pos, const float pitch, const float yaw) {
  glm::mat4 transform_x = glm::eulerAngleX(glm::radians(pitch));
  glm::mat4 transform_y = glm::eulerAngleY(glm::radians(yaw));
  view = glm::translate(transform_x * transform_y, -pos);
}
