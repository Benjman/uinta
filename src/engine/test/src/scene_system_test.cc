#include "uinta/scene/scene_system.h"

#include <vector>

#include "./utils.h"
#include "uinta/engine/engine.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {

class SceneSystemTest : public UintaTestF {};

TEST_F(SceneSystemTest, SceneAddedDispatchedForRootAndNestedScenes) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  std::vector<const Scene*> added;
  engine.scenes()->addListener<SceneEvent::SceneAdded>([&](const SceneAddedEvent& e) { added.push_back(e.scene); });

  auto* root = engine.addScene<MockScene>();
  auto* child = root->addScene<MockScene>();
  auto* grandchild = child->addScene<MockScene>();

  EXPECT_EQ((std::vector<const Scene*>{root, child, grandchild}), added);
  EXPECT_EQ(root, engine.scenes()->active());
}

TEST_F(SceneSystemTest, StateAndLayerChangesDispatchedFromNestedScenes) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  auto* grandchild = root->addScene<MockScene>()->addScene<MockScene>();

  const Scene* stateScene = nullptr;
  SceneState oldState = SceneState::Error;
  SceneState newState = SceneState::Error;
  engine.scenes()->addListener<SceneEvent::StateChange>([&](const SceneStateChangeEvent& e) {
    stateScene = e.scene;
    oldState = e.oldState;
    newState = e.newState;
  });

  const Scene* layerScene = nullptr;
  SceneLayer newLayer = SceneLayer::Simulation;
  engine.scenes()->addListener<SceneEvent::LayerChange>([&](const SceneLayerChangeEvent& e) {
    layerScene = e.scene;
    newLayer = e.newLayer;
  });

  grandchild->state(SceneState::Pause);
  grandchild->layer(SceneLayer::Debug);

  EXPECT_EQ(grandchild, stateScene);
  EXPECT_EQ(SceneState::Running, oldState);
  EXPECT_EQ(SceneState::Pause, newState);
  EXPECT_EQ(grandchild, layerScene);
  EXPECT_EQ(SceneLayer::Debug, newLayer);
}

TEST_F(SceneSystemTest, CompletedChildRemovedOnFlush) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  std::vector<const Scene*> removed;
  engine.scenes()->addListener<SceneEvent::SceneRemoved>(
      [&](const SceneRemovedEvent& e) { removed.push_back(e.scene); });

  auto* root = engine.addScene<MockScene>();
  auto* child = root->addScene<MockScene>();
  root->addScene<MockScene>();

  child->state(SceneState::Complete);
  EXPECT_EQ(2, root->children().size()) << "Removal must be deferred until flush.";
  EXPECT_TRUE(removed.empty());

  engine.scenes()->flush();
  EXPECT_EQ(1, root->children().size());
  EXPECT_EQ((std::vector<const Scene*>{child}), removed);
  EXPECT_EQ(0, engine.scenes()->pendingRemovals());
}

TEST_F(SceneSystemTest, ErroredChildRemovedOnFlush) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  root->addScene<MockScene>()->state(SceneState::Error);

  engine.scenes()->flush();
  EXPECT_EQ(0, root->children().size());
}

TEST_F(SceneSystemTest, ResumedSceneNotRemoved) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  auto* child = root->addScene<MockScene>();

  child->state(SceneState::Complete);
  child->state(SceneState::Running);
  engine.scenes()->flush();

  EXPECT_EQ(1, root->children().size());
}

TEST_F(SceneSystemTest, CompletingParentAndChildTogether) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  std::vector<const Scene*> removed;
  engine.scenes()->addListener<SceneEvent::SceneRemoved>(
      [&](const SceneRemovedEvent& e) { removed.push_back(e.scene); });

  auto* root = engine.addScene<MockScene>();
  auto* parent = root->addScene<MockScene>();
  auto* child = parent->addScene<MockScene>();

  // Child is pending last, so it would be processed first; complete it first
  // too, so the parent is processed first and destroys the pending child.
  child->state(SceneState::Complete);
  parent->state(SceneState::Complete);
  EXPECT_EQ(2, engine.scenes()->pendingRemovals());

  engine.scenes()->flush();
  EXPECT_EQ(0, root->children().size());
  EXPECT_EQ((std::vector<const Scene*>{parent}), removed);
  EXPECT_EQ(0, engine.scenes()->pendingRemovals());
}

TEST_F(SceneSystemTest, CompletedRootPoppedAndNextBecomesActive) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* first = engine.addScene<MockScene>();
  first->addScene<MockScene>()->state(SceneState::Complete);
  auto* second = engine.addScene<MockScene>();

  EXPECT_EQ(first, engine.scenes()->active());
  first->state(SceneState::Complete);
  engine.scenes()->flush();

  EXPECT_EQ(second, engine.scenes()->active());
  EXPECT_EQ(1, engine.scenes()->roots().size());
  EXPECT_EQ(0, engine.scenes()->pendingRemovals());
}

TEST_F(SceneSystemTest, UnownedRootIgnoredAndForgottenOnDestruction) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  {
    MockScene root(&engine);
    root.state(SceneState::Complete);
    EXPECT_EQ(1, engine.scenes()->pendingRemovals());
  }
  EXPECT_EQ(0, engine.scenes()->pendingRemovals());
  engine.scenes()->flush();
}

}  // namespace uinta
