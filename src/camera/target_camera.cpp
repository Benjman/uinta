#include <glm/ext/matrix_transform.hpp>
#include <uinta/camera/target_camera.hpp>

namespace uinta {
void processInput(TargetCamera&, const RunnerState&);
}

void uinta::update(TargetCamera& cam, const RunnerState& state) {
  processInput(cam, state);
}

glm::mat4 uinta::getViewMatrix(const TargetCamera& cam) {
  auto result = glm::rotate(glm::mat4(1), glm::radians(cam.pitch), WORLD_RIGHT);
  result = glm::rotate(result, glm::radians(cam.yaw), WORLD_FORWARD);
  // result = glm::rotate(result, glm::radians(0.0f), WORLD_UP); // intended for roll
  return glm::translate(result, -cam.position);
}

void uinta::processInput(TargetCamera& cam, const RunnerState& state) {
}
