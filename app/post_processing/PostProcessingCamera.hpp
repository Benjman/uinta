#ifndef UINTA_APP_POST_PROCESSING_CAMERA_HPP
#define UINTA_APP_POST_PROCESSING_CAMERA_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <uinta/camera/basic_camera.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

struct PostProcessingCamera : BasicCamera {
  glm::mat4 view;
  float cameraRadius = 5.0;
  float cameraSpeed = 0.25;

  PostProcessingCamera() {
    position = glm::normalize(glm::vec3(0, 1, 1)) * cameraRadius;
  }

  inline void tick(const RunnerState& state) {
    position.x = sin(state.runtime * cameraSpeed) * cameraRadius;
    position.z = cos(state.runtime * cameraSpeed) * cameraRadius;
  }

  inline void updateView(const RunnerState& state) {
    view = glm::lookAt(position, glm::vec3(0), WORLD_UP);
  }
};

}  // namespace uinta

#endif  // UINTA_APP_POST_PROCESSING_CAMERA_HPP
