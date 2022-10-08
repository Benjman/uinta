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
    guides.init(fileManager);
    return true;
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();
    glm::mat4 view(1.0);
    view = glm::rotate(view, glm::radians(90.f), glm::vec3(1, 0, 0));
    view = glm::translate(view, -WORLD_UP);

    float orthoSize = 1.0;
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
  return uinta::CartesianRunner().run();
}
