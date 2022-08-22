#include <algorithm>  // std::min, std::clamp
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <uinta/math.hpp>

#include "./math/running_avg.cpp"
#include "./math/smooth_float.cpp"

using namespace uinta;

void uinta::genViewMatrix(glm::mat4& view, const glm::vec3& pos, const float pitchDeg, const float yawDeg) {
  glm::mat4 transform_x = glm::eulerAngleX(glm::radians(pitchDeg));
  glm::mat4 transform_y = glm::eulerAngleY(glm::radians(yawDeg));
  view = glm::translate(transform_x * transform_y, -pos);
}
