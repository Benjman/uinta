#include "uinta/scene.h"

#include <gtest/gtest.h>

namespace uinta {

struct TestSceneA : Scene {
 public:
  TestSceneA() noexcept : Scene(Layer::Debug) {}
};

struct TestSceneB : Scene {
  TestSceneB() noexcept : Scene(Layer::Debug) { addScene<TestSceneA>(); }
};

struct TestSceneC : Scene {
  TestSceneC() noexcept : Scene(Layer::Debug) { addScene<TestSceneB>(); }
};

TEST(Scene, findScene) {
  TestSceneC scene;
  ASSERT_TRUE(Scene::FindScene<TestSceneB>(&scene))
      << "TestSceneB was expected to be a child scene of TestSceneC";
  ASSERT_FALSE(Scene::FindScene<TestSceneC>(&scene))
      << "TestSceneB was not expected to be a child scene of TestSceneC";
  ASSERT_TRUE(Scene::FindScene<TestSceneA>(&scene))
      << "TestSceneA was expected to be a child scene of TestSceneB which was "
         "a child scene of TestSceneC";
  ASSERT_FALSE(Scene::FindScene<TestSceneA>(&scene, false))
      << "TestSceneA was not expected to be found as it was a child scene of "
         "TestSceneB which was a child scene of TestSceneC, and "
         "`includChildren` flag was false";
}

}  // namespace uinta
