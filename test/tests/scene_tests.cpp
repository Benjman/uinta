#include <gtest/gtest.h>

#define private public
#define protected public
#include <uinta/scene/scene.hpp>

using namespace uinta;

TEST(SceneTest, null_renderer) {
  SceneDependencies dependencies;
  dependencies.renderer = nullptr;
  ASSERT_DEATH({ Scene(std::move(dependencies)); }, "Renderer must be initialized") << "Application was expected to die.";
}
