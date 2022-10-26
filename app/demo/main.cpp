#include "../utils/glfw_runner.hpp"

namespace uinta {

class DemoRunner : public GlfwRunner {
 public:
  DemoRunner() : GlfwRunner("Demo", 1920, 1080) {
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner().run();
}
