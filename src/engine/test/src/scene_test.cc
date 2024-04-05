#include "uinta/scene.h"

#include <gtest/gtest.h>

#include <string>

namespace uinta {

// struct TestSceneA : Scene {
//  public:
//   explicit TestSceneA(Scene* parent) noexcept
//       : Scene(parent, nullptr, Layer::Debug) {}
//
//   std::string name() const noexcept override { return "TestSceneA"; }
// };
//
// struct TestSceneB : Scene {
//   explicit TestSceneB(Scene* parent) noexcept
//       : Scene(parent, nullptr, Layer::Debug) {
//     addScene<TestSceneA>();
//   }
//
//   std::string name() const noexcept override { return "TestSceneB"; }
// };
//
// struct TestSceneC : Scene {
//   TestSceneC() noexcept : Scene(nullptr, nullptr, Layer::Debug) {
//     addScene<TestSceneB>();
//   }
//
//   std::string name() const noexcept override { return "TestSceneC"; }
// };

// // FIXME: Logic has changed for finding scenes. Tests need to reflect these
// // changes.
// TEST(Scene, findScene) {
//   TestSceneC scene;
//   ASSERT_TRUE(Scene::FindScene<TestSceneB>(&scene))
//       << "TestSceneB was expected to be a child scene of TestSceneC";
//   ASSERT_FALSE(Scene::FindScene<TestSceneC>(&scene))
//       << "TestSceneB was not expected to be a child scene of TestSceneC";
//   ASSERT_TRUE(Scene::FindScene<TestSceneA>(&scene))
//       << "TestSceneA was expected to be a child scene of TestSceneB which was
//       "
//          "a child scene of TestSceneC";
//   ASSERT_FALSE(Scene::FindScene<TestSceneA>(&scene))
//       << "TestSceneA was not expected to be found as it was a child scene of
//       "
//          "TestSceneB which was a child scene of TestSceneC, and "
//          "`includChildren` flag was false";
// }

}  // namespace uinta
