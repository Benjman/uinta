#include "uinta/scene/scene_system.h"

#include <string>
#include <utility>
#include <vector>

#include "./utils.h"
#include "uinta/engine/engine.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {

class SceneSystemTest : public UintaTestF {};

namespace {

// Logs its id on destruction, along with how many children it still had.
class DestroyLogScene : public Scene {
 public:
  DestroyLogScene(Engine* engine, std::vector<std::string>* log, std::string id) noexcept
      : Scene(engine), log_(log), id_(std::move(id)) {}

  DestroyLogScene(Scene* parent, std::vector<std::string>* log, std::string id) noexcept
      : Scene(parent), log_(log), id_(std::move(id)) {}

  ~DestroyLogScene() noexcept override { log_->push_back(id_ + ":" + std::to_string(children().size())); }

 private:
  std::vector<std::string>* log_;
  std::string id_;
};

void buildTree(Scene* root, std::vector<std::string>* log) noexcept {
  auto* a = root->addScene<DestroyLogScene>(log, "a");
  a->addScene<DestroyLogScene>(log, "a1");
  a->addScene<DestroyLogScene>(log, "a2");
  root->addScene<DestroyLogScene>(log, "b");
}

}  // namespace

TEST_F(SceneSystemTest, FlushDestroysSubtreeBottomUp) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  std::vector<std::string> log;
  auto* root = engine.addScene<DestroyLogScene>(&log, "root");
  buildTree(root, &log);

  root->state(SceneState::Complete);
  engine.scenes()->flush();

  EXPECT_EQ((std::vector<std::string>{"b:0", "a2:0", "a1:0", "a:0", "root:0"}), log);
}

TEST_F(SceneSystemTest, ShutdownDestroysSubtreeBottomUp) {
  MockPlatform platform;
  std::vector<std::string> log;
  {
    auto engine = makeEngine(&platform);
    buildTree(engine.addScene<DestroyLogScene>(&log, "root"), &log);
  }

  EXPECT_EQ((std::vector<std::string>{"b:0", "a2:0", "a1:0", "a:0", "root:0"}), log);
}

TEST_F(SceneSystemTest, FirstRootIsActive) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  EXPECT_EQ(nullptr, engine.scenes()->active());

  auto* root = engine.addScene<MockScene>();
  engine.addScene<MockScene>();

  EXPECT_EQ(root, engine.scenes()->active());
}

TEST_F(SceneSystemTest, CompletedChildRemovedOnFlush) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  auto* child = root->addScene<MockScene>();
  auto* sibling = root->addScene<MockScene>();

  child->state(SceneState::Complete);
  EXPECT_EQ(2, root->children().size()) << "Removal must be deferred until flush.";

  engine.scenes()->flush();
  ASSERT_EQ(1, root->children().size());
  EXPECT_EQ(sibling, root->children().front().get());
}

TEST_F(SceneSystemTest, CompletedGrandchildRemovedOnFlush) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  auto* child = root->addScene<MockScene>();
  child->addScene<MockScene>()->state(SceneState::Complete);

  engine.scenes()->flush();
  EXPECT_EQ(1, root->children().size());
  EXPECT_EQ(0, child->children().size());
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
  auto* root = engine.addScene<MockScene>();
  auto* parent = root->addScene<MockScene>();
  auto* child = parent->addScene<MockScene>();

  child->state(SceneState::Complete);
  parent->state(SceneState::Complete);

  engine.scenes()->flush();
  EXPECT_EQ(0, root->children().size());
}

TEST_F(SceneSystemTest, CompletedRootPoppedAndNextBecomesActive) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* first = engine.addScene<MockScene>();
  first->addScene<MockScene>();
  auto* second = engine.addScene<MockScene>();

  EXPECT_EQ(first, engine.scenes()->active());
  first->state(SceneState::Complete);
  engine.scenes()->flush();

  EXPECT_EQ(second, engine.scenes()->active());
  EXPECT_EQ(1, engine.scenes()->roots().size());
}

TEST_F(SceneSystemTest, PausedSceneSkipsSubtree) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto* root = engine.addScene<MockScene>();
  auto* paused = root->addScene<MockScene>();
  auto* child = paused->addScene<MockScene>();
  paused->state(SceneState::Pause);

  int rootTicks = 0;
  int pausedTicks = 0;
  int childTicks = 0;
  root->onTick = [&](time_t) { ++rootTicks; };
  paused->onTick = [&](time_t) { ++pausedTicks; };
  child->onTick = [&](time_t) { ++childTicks; };

  engine.scenes()->advance<EngineStage::Tick>(0);

  EXPECT_EQ(1, rootTicks);
  EXPECT_EQ(0, pausedTicks);
  EXPECT_EQ(0, childTicks);
}

}  // namespace uinta
