#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/grid.hpp>
#include <uinta/scene.hpp>
#include <uinta/target_camera.hpp>

namespace uinta {

class DemoScene : public Scene {
 public:
  DemoScene(Runner& runner) : Scene("Demo", runner, Scene::Layer::Simulation) {
    if (auto error = runner.add_scene(std::make_unique<GridScene>(runner)); error) throw UintaException(error);
    m_camera.dist(30);
    m_camera.pitch(15);
    runner.subscribe(RunnerEvents::WindowSizeChanged, [&](const Event* event) {
      if (const auto* e = static_cast<const WindowEvent*>(event); e) m_camera.aspect_ratio(e->window.aspect_ratio);
    });
  }

  ~DemoScene() override = default;

  void tick(const RunnerState& state, const InputState& input) override {
    Scene::tick(state, input);
    m_camera.update(state, input);
  }

  uinta_error_code init() override {
    return transition(State::Running);
  }

  TargetCamera* camera() noexcept override {
    return &m_camera;
  }

 private:
  TargetCamera m_camera;
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  using namespace uinta;
  auto runner = GlfwRunner("Demo", argc, argv);
  if (auto error = runner.add_scene(std::make_unique<DemoScene>(runner)); error) throw UintaException(error);
  return runner.run();
}
