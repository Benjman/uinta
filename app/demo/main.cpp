#include <uinta/glfw_utils/glfw_runner.hpp>

namespace uinta {

class DemoRunner : public GlfwRunner {
 public:
  entt::entity cube;

  DemoRunner(const int argc, const char** argv) : GlfwRunner("Demo", argc, argv) {
  }

  bool doInit() override {
    if (!GlfwRunner::doInit()) return false;

    cube = scene.addEntity({
        "models/cube.obj",
        {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}},
    });

    return true;
  }

  void doRender(const RunnerState& state) override {
    GlfwRunner::doRender(state);
    scene.startRender(this, state);
    scene.render(cube, registry);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner(argc, argv).run();
}
