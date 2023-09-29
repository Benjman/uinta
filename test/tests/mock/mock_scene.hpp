#ifndef UINTA_TEST_SCENE_HPP
#define UINTA_TEST_SCENE_HPP

#include <uinta/scene.hpp>

#include "./mock_runner.hpp"

namespace uinta {

class MockScene : public Scene {
 public:
  MockScene(MockRunner& runner, Scene::Layer layer = Layer::Debug, const std::string& name = "")
      : Scene(getUniqueTestName(name), runner, layer) {
  }

  ~MockScene() override = default;

  std::function<uinta_error_code()> on_init;
  uinta_error_code init() override {
    if (on_init) return on_init();
    return transition(State::Running);
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_SCENE_HPP
