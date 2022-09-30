#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../utils/utils.hpp"

namespace uinta {

class CartesianRunner final : public GlfwRunner {
  CartesianGuides guides;

 public:
  CartesianRunner() : GlfwRunner("Cartesian Guides", 1920, 1080) {
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setBackground(glm::vec3(38, 70, 83) / 255.0f);
  }

  bool doInit() override {
    guides.init(fileManager, display);
    return true;
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();

    glm::vec3 up = {0, 1, 0};
    float cameraDist = 20;
    float pitch = -1;
    float yaw = 5 * cos(state.runtime);
    float y = cameraDist * sin(glm::radians(-pitch));
    float z = cameraDist * cos(glm::radians(-pitch));
    float x = cameraDist * sin(glm::radians(-yaw));
    auto view = glm::lookAt({x, y, z}, glm::vec3(0), up);
    if (state.tick % 20 == 0) {
      SPDLOG_INFO("Camera pos: ({}, {}, {})\tPitch: {}\tYaw: {}", x, y, z, pitch, yaw);
    }

    float orthoSize = 5.0;
    auto proj = glm::ortho(-orthoSize * display.aspectRatio,  // left
                           orthoSize * display.aspectRatio,   // right
                           -orthoSize,                        // bottom
                           orthoSize,                         // top
                           0.1f,                              // near
                           100.0f);                           // far

    guides.doRender(state, proj * view);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::CartesianRunner().run();
}
