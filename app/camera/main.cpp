#include <glm/gtx/euler_angles.hpp>

#include "../utils/utils.hpp"

namespace uinta {

class CameraRunner : public GlfwRunner {
 public:
  TargetCamera cam{1};
  CartesianGrid grid;

  CameraRunner() : GlfwRunner("Camera", 1920, 1080) {
    setClearMask(GL_COLOR_BUFFER_BIT);
    setBackground(glm::vec3(38, 70, 83) / 255.0f);
  }

  bool doInit() override {
    grid.init(fileManager);
    return true;
  }

  void doTick(const RunnerState& state) override {
    // cam.position.x = 2 * cos(state.runtime);
    // cam.position.z = 2 * sin(state.runtime);
    // cam.pitch = 45 + 45 * sin(state.runtime);
    // cam.yaw = 45 + 45 * cos(state.runtime);
    update(cam, state);
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();

    // clang-format off
    float orthoSize = 3.0;
    auto proj = glm::ortho(
        -orthoSize * display.aspectRatio,
         orthoSize * display.aspectRatio,
        -orthoSize,
         orthoSize,
         0.1f,
         100.0f
        );
    // clang-format on

    // proj = glm::perspective(glm::radians(45.f), display.aspectRatio, 0.1f, 100.0f);

    grid.render(proj * getViewMatrix(cam));
    imgui::view::camera(cam);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::CameraRunner().run();
}
