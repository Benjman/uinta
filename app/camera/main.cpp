#include <glm/gtx/euler_angles.hpp>

#include "../utils/utils.hpp"

namespace uinta {

class CameraRunner : public GlfwRunner {
 public:
  TargetCamera cam;
  CartesianGrid grid;

  CameraRunner() : GlfwRunner("Camera", 1920, 1080) {
    setClearMask(GL_COLOR_BUFFER_BIT);
    setBackground(glm::vec3(38, 70, 83) / 255.0f);

    force(cam.angle, 45);
    force(cam.pitch, 45);
    force(cam.dist, 5);
  }

  bool doInit() override {
    grid.init(fileManager);
    return true;
  }

  void doTick(const RunnerState& state) override {
    update(cam, state);
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();
    auto proj = glm::perspective(glm::radians(45.f), display.aspectRatio, 0.1f, 100.0f);
    grid.render(proj * getViewMatrix(cam));
    imgui::view::camera(cam);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::CameraRunner().run();
}
