#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/math/direction.hpp>

#include "../utils/utils.hpp"

namespace uinta {

class Camera3dRunner : public GlfwRunner {
 public:
  CartesianGuides guides;

  Camera3dRunner() : GlfwRunner("Camera", 1920, 1080) {
    setClearMask(GL_COLOR_BUFFER_BIT);
    setBackground(glm::vec3(38, 70, 83) / 255.0f);
  }

  bool doInit() override {
    guides.init(fileManager);
    return true;
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();

    // cam.yaw = 90 * sin(state.runtime);

    auto view = glm::rotate(glm::mat4(1.0), glm::radians(90.f), glm::vec3(1, 0, 0));
    view = glm::translate(view, -WORLD_UP);
    view = glm::translate(view, WORLD_FORWARD * (float)sin(state.runtime));
    view = glm::translate(view, glm::cross(WORLD_FORWARD, WORLD_UP) * (float)cos(state.runtime));

    float orthoSize = 6.0;
    // clang-format off
    auto proj = glm::ortho(
        -orthoSize * display.aspectRatio,
         orthoSize * display.aspectRatio,
        -orthoSize,
         orthoSize,
         0.1f,
         100.0f
        );
    // clang-format on

    guides.render(proj * view);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::Camera3dRunner().run();
}
