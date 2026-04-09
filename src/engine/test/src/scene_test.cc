#include "uinta/scene/scene.h"

#include <optional>

#include "./utils.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {

size_t MockScene::TestSceneId = 0;

class SceneTest : public UintaTestF {};

TEST_F(SceneTest, FindScene) {
  // Define distinct scene types for testing the scene hierarchy
  struct ChildScene : Scene {
    explicit ChildScene(Engine* engine,
                        SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(engine, layer) {}
    explicit ChildScene(Scene* parent,
                        SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(parent, layer) {}
  };
  struct LeafScene : Scene {
    explicit LeafScene(Engine* engine,
                       SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(engine, layer) {}
    explicit LeafScene(Scene* parent,
                       SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(parent, layer) {}
  };
  struct UnusedScene : Scene {
    explicit UnusedScene(Engine* engine,
                         SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(engine, layer) {}
    explicit UnusedScene(Scene* parent,
                         SceneLayer layer = SceneLayer::Debug) noexcept
        : Scene(parent, layer) {}
  };

  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene root(&engine);
  ASSERT_EQ("uinta::MockScene", root.name());

  // Test finding the root scene itself
  auto foundRoot = root.findScene<MockScene>();
  ASSERT_TRUE(foundRoot.has_value())
      << "Should find root scene when searching from root.";
  ASSERT_EQ(&root, foundRoot.value());

  // Add child scenes
  auto* child1 = root.addScene<ChildScene>();
  auto* child2 = root.addScene<ChildScene>();
  ASSERT_NE(nullptr, child1);
  ASSERT_NE(nullptr, child2);
  ASSERT_EQ("uinta::SceneTest_FindScene_Test::TestBody()::ChildScene",
            child1->name());

  // Test finding child scene from root
  auto foundChild = root.findScene<ChildScene>();
  ASSERT_TRUE(foundChild.has_value()) << "Should find ChildScene from root.";
  ASSERT_EQ(child1, foundChild.value())
      << "Should return first matching child scene.";
  ASSERT_EQ(&root, foundChild.value()->parent())
      << "root expected to be ChildScene's parent.";

  // Add leaf scenes to first child
  auto* leaf1 = child1->addScene<LeafScene>();
  auto* leaf2 = child1->addScene<LeafScene>();
  ASSERT_NE(nullptr, leaf1);
  ASSERT_NE(nullptr, leaf2);
  ASSERT_EQ("uinta::SceneTest_FindScene_Test::TestBody()::LeafScene",
            leaf1->name());

  // Test finding leaf scene from root (recursive search)
  auto foundLeaf = root.findScene<LeafScene>(true);
  ASSERT_TRUE(foundLeaf.has_value())
      << "Should find LeafScene from root with recursive search.";
  ASSERT_EQ(leaf1, foundLeaf.value());
  ASSERT_EQ(child1, foundLeaf.value()->parent())
      << "ChildScene expected to be LeafScene's parent.";
  ASSERT_EQ(&root, foundLeaf.value()->parent()->parent())
      << "Root expected to be LeafScene's grandparent.";

  // Test finding leaf scene from child
  auto foundLeafFromChild = child1->findScene<LeafScene>();
  ASSERT_TRUE(foundLeafFromChild.has_value())
      << "Should find LeafScene from its parent.";
  ASSERT_EQ(leaf1, foundLeafFromChild.value());

  // Test finding leaf scene from sibling child (should search up to root)
  auto foundLeafFromSibling = child2->findScene<LeafScene>(true);
  ASSERT_TRUE(foundLeafFromSibling.has_value())
      << "Should find LeafScene from sibling via root.";
  ASSERT_EQ(leaf1, foundLeafFromSibling.value());

  // Test non-recursive search doesn't find distant scenes
  auto notFoundNonRecursive = child2->findScene<LeafScene>(false);
  ASSERT_FALSE(notFoundNonRecursive.has_value())
      << "Should not find LeafScene from sibling without recursive search.";

  // Test finding non-existent scene type
  auto notFound = root.findScene<UnusedScene>();
  ASSERT_FALSE(notFound.has_value())
      << "Should not find scene type that doesn't exist in hierarchy.";
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
