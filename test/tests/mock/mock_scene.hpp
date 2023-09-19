#ifndef UINTA_TEST_SCENE_HPP
#define UINTA_TEST_SCENE_HPP

#include <uinta/scene/scene.hpp>

#include "./mock_runner.hpp"

namespace uinta {

class MockScene : public Scene {
 public:
  MockScene(MockRunner runner = {}) : Scene(runner) {
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_SCENE_HPP
