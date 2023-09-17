#include <gtest/gtest.h>

#include "./utils/test_runner.hpp"

#define private public
#define protected public
#include <uinta/scene/scene.hpp>

using namespace uinta;

TEST(SceneTest, null_renderer) {
  TestRunner runner;
  SceneDependencies dependencies;
  dependencies.renderer = nullptr;
  ASSERT_DEATH({ Scene(runner, std::move(dependencies)); }, "Renderer must be initialized") << "Application was expected to die.";
}
