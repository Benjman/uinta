#include <uinta/error.hpp>
#include <uinta/glfw/glfw_runner.hpp>

namespace uinta {

class DemoRunner : public GlfwRunner {
 public:
  entt::entity entity;

  DemoRunner(const int argc, const char** argv) : GlfwRunner("Demo", argc, argv) {
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner(argc, argv).run();
}
