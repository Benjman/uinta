#include "uinta/scene.h"

#include <gtest/gtest.h>

namespace uinta {

struct TestSceneA : Scene {
 public:
  explicit TestSceneA(Scene* parent) noexcept : Scene(parent, Layer::Debug) {}
};

struct TestSceneB : Scene {
  explicit TestSceneB(Scene* parent) noexcept : Scene(parent, Layer::Debug) {
    addScene<TestSceneA>(this);
  }
};

struct TestSceneC : Scene {
  TestSceneC() noexcept : Scene(static_cast<Scene*>(nullptr), Layer::Debug) {
    addScene<TestSceneB>(this);
  }
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
  ASSERT_FALSE(Scene::FindScene<TestSceneA>(&scene))
      << "TestSceneA was not expected to be found as it was a child scene of "
         "TestSceneB which was a child scene of TestSceneC, and "
         "`includChildren` flag was false";
}

}  // namespace uinta
