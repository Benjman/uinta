#include <uinta/glfw_utils/glfw_runner.hpp>

namespace uinta {

class DemoRunner : public GlfwRunner {
 public:
  DemoRunner(const int argc, const char** argv) : GlfwRunner("Demo", 1920, 1080, argc, argv) {
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner(argc, argv).run();
}
