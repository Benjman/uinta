#include <glm/gtx/euler_angles.hpp>

#include "../utils/utils.hpp"

namespace uinta {

class CameraRunner : public GlfwRunner {
 public:
  CameraRunner() : GlfwRunner("Camera", 1920, 1080) {
    setBackground(glm::vec3(38, 70, 83) / 255.0f);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::CameraRunner().run();
}
