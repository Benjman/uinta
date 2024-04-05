#include "uinta/scene/scene.h"

#include <optional>

#include "./utils.h"
#include "absl/strings/str_format.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {

size_t MockScene::TestSceneId = 0;

class SceneTest : public UintaTestF {};

TEST_F(SceneTest, FindScene) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene root(&engine);
  ASSERT_EQ("Scene0", root.name());

  constexpr auto childNodes = 10;
  for (auto i = 0; i < childNodes; i++) {
    root.addScene<MockScene>();
  }

  constexpr auto leafNodes = 10;
  for (auto& scene : root.children()) {
    for (auto ii = 0; ii < leafNodes; ii++) {
      scene->addScene<MockScene>();
    }
  }

  auto sceneThirtyTwo = root.findScene<MockScene>("Scene32", true);
  ASSERT_TRUE(sceneThirtyTwo.has_value());
  ASSERT_NE(std::nullopt, sceneThirtyTwo);

  auto sceneOneFromPrevious =
      sceneThirtyTwo.value()->findScene<MockScene>("Scene32", true);
  ASSERT_TRUE(sceneOneFromPrevious.has_value());
  ASSERT_NE(std::nullopt, sceneOneFromPrevious);

  ASSERT_TRUE(root.findScene<MockScene>("Scene1").has_value())
      << "Scene1 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene2").has_value())
      << "Scene2 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene3").has_value())
      << "Scene3 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene4").has_value())
      << "Scene4 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene5").has_value())
      << "Scene5 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene6").has_value())
      << "Scene6 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene7").has_value())
      << "Scene7 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene8").has_value())
      << "Scene8 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene9").has_value())
      << "Scene9 expected to be a child scene of root.";

  ASSERT_TRUE(root.findScene<MockScene>("Scene10").has_value())
      << "Scene10 expected to be a child scene of root.";

  ASSERT_FALSE(
      root.findScene<MockScene>(
              absl::StrFormat("Scene%i", childNodes * (leafNodes + 1) + 1))
          .has_value())
      << "This scene does not exist, and is not expected to be found.";

  static constexpr std::string Scene1Name = "Scene1";
  static constexpr std::string Scene11Name = "Scene11";

  root.findScene<MockScene>();
  auto scene1 = root.findScene<MockScene>(Scene1Name);
  ASSERT_TRUE(scene1.has_value()) << "Scene1 expected be a child of the root.";
  ASSERT_EQ(Scene1Name, scene1.value()->name());
  ASSERT_EQ(&root, scene1.value()->parent())
      << "root expected to be Scene1's parent.";
  auto scene11 = root.findScene<MockScene>(Scene11Name, true);
  ASSERT_TRUE(scene11.has_value())
      << "Scene11 should be expected to be found from the root, as it's part "
         "of root's children.";
  ASSERT_NE(&root, scene11.value()->parent())
      << "Scene11's parent should be Scene01's, not root.";
  ASSERT_EQ(scene1, scene11.value()->parent())
      << "Scene11's parent expected to be " << scene1.value()->name();
  ASSERT_EQ(&root, scene11.value()->parent()->parent());
}

TEST_F(SceneTest, FindSystem) {
  struct RootSystem : System {};
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene root(&engine);
  root.addSystem<RootSystem>();

  ASSERT_NE(std::nullopt, root.findSystem<RootSystem>())
      << "Root should return its own system.";

  auto branch = root.addScene<MockScene>();
  struct BranchSystem : System {};
  branch->addSystem<BranchSystem>();
  ASSERT_NE(std::nullopt, branch->findSystem<BranchSystem>())
      << "Branch should return its own system.";
  ASSERT_NE(std::nullopt, branch->findSystem<RootSystem>())
      << "Branch should return a root system";

  auto branchLeaf = branch->addScene<MockScene>();
  struct BranchLeafSystem : System {};
  branchLeaf->addSystem<BranchLeafSystem>();
  ASSERT_NE(std::nullopt, branchLeaf->findSystem<BranchLeafSystem>())
      << "BranchLeaf should return its own system.";
  ASSERT_NE(std::nullopt, branchLeaf->findSystem<RootSystem>())
      << "BranchLeaf should return a root system";
  ASSERT_NE(std::nullopt, branchLeaf->findSystem<BranchSystem>())
      << "BranchLeaf should return a branch system";
}

TEST_F(SceneTest, RootSceneExecuting) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) { engine.state().isClosing(true); });

  auto* scene = engine.addScene<MockScene>();

  bool preTickCalled = false;
  scene->onPreTick = [&preTickCalled](const auto&) { preTickCalled = true; };

  bool tickCalled = false;
  scene->onTick = [&tickCalled](const auto&) { tickCalled = true; };

  bool postTickCalled = false;
  scene->onPostTick = [&postTickCalled](const auto&) { postTickCalled = true; };

  bool preRenderCalled = false;
  scene->onPreRender = [&preRenderCalled](const auto&) {
    preRenderCalled = true;
  };

  bool renderCalled = false;
  scene->onRender = [&renderCalled](const auto&) { renderCalled = true; };

  bool postRenderCalled = false;
  scene->onPostRender = [&postRenderCalled](const auto&) {
    postRenderCalled = true;
  };

  engine.run();

  ASSERT_TRUE(preTickCalled);
  ASSERT_TRUE(tickCalled);
  ASSERT_TRUE(postTickCalled);
  ASSERT_TRUE(preRenderCalled);
  ASSERT_TRUE(renderCalled);
  ASSERT_TRUE(postRenderCalled);
}

}  // namespace uinta
