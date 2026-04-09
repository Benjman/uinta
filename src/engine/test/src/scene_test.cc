#include "uinta/scene/scene.h"

#include "./utils.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {

size_t MockScene::TestSceneId = 0;

class SceneTest : public UintaTestF {};

TEST_F(SceneTest, Hierarchy) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene root(&engine);
  ASSERT_EQ("uinta::MockScene", root.name());

  auto* child = root.addScene<MockScene>();
  auto* grandchild = child->addScene<MockScene>();

  EXPECT_EQ(1, root.children().size());
  EXPECT_EQ(child, root.children().front().get());
  EXPECT_EQ(&root, child->parent());
  EXPECT_EQ(child, grandchild->parent());
  EXPECT_EQ(&engine, grandchild->engine());
  EXPECT_EQ(SceneState::Running, grandchild->state());
}

TEST_F(SceneTest, RootSceneExecuting) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  platform.onSwapBuffers = [&]() {
    engine.state().isClosing(true);
    return OkStatus();
  };

  auto* scene = engine.addScene<MockScene>();

  bool preTickCalled = false;
  scene->onPreTick = [&preTickCalled](const auto&) { preTickCalled = true; };

  bool tickCalled = false;
  scene->onTick = [&tickCalled](const auto&) { tickCalled = true; };

  bool postTickCalled = false;
  scene->onPostTick = [&postTickCalled](const auto&) { postTickCalled = true; };

  bool preRenderCalled = false;
  scene->onPreRender = [&preRenderCalled](const auto&) { preRenderCalled = true; };

  bool renderCalled = false;
  scene->onRender = [&renderCalled](const auto&) { renderCalled = true; };

  bool renderUiCalled = false;
  scene->onRenderUi = [&renderUiCalled](const auto&) { renderUiCalled = true; };

  bool renderDebugCalled = false;
  scene->onRenderDebug = [&renderDebugCalled](const auto&) { renderDebugCalled = true; };

  bool postRenderCalled = false;
  scene->onPostRender = [&postRenderCalled](const auto&) { postRenderCalled = true; };

  engine.run();

  ASSERT_TRUE(preTickCalled);
  ASSERT_TRUE(tickCalled);
  ASSERT_TRUE(postTickCalled);
  ASSERT_TRUE(preRenderCalled);
  ASSERT_TRUE(renderCalled);
  ASSERT_TRUE(renderUiCalled);
  ASSERT_TRUE(renderDebugCalled);
  ASSERT_TRUE(postRenderCalled);
}

}  // namespace uinta
