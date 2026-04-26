// Tests for PostProcessor pipeline behavior using the mock GL. We exercise
// the state-machine (begin/end simulation, pass enable/disable, ping-pong
// routing, fallback) without needing a real GL context.

#include "uinta/post_process/post_processor.h"

#include <gtest/gtest.h>

#include <vector>

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/fbo.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/post_process/pass.h"
#include "uinta/post_process/passes/passthrough_pass.h"
#include "uinta/post_process/render_target.h"
#include "uinta/texture.h"

#include "../utils.h"

namespace uinta {

namespace {

// Recording pass that notes every execution with the `input` / `output` it
// saw. Allows verifying ping-pong routing.
class RecordingPass : public PostProcessPass {
 public:
  struct Record {
    const Texture* input;
    const Fbo* output;
  };

  RecordingPass(const OpenGLApi* /*gl*/, std::vector<Record>* log) noexcept
      : log_(log) {}

  void execute(const PassContext& ctx) noexcept override {
    log_->push_back({ctx.input, ctx.output});
  }

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Recording";
  }

 private:
  std::vector<Record>* log_;
};

}  // namespace

struct PostProcessorTest : public UintaTestF {
 protected:
  void SetUp() override {
    UintaTestF::SetUp();
    gl.onCheckFramebufferStatus = [](auto) -> GLenum {
      return GL_FRAMEBUFFER_COMPLETE;
    };
    gl.onGetBooleanv = [](auto, GLboolean* out) { *out = GL_TRUE; };
    gl.onGetIntegerv = [](auto, GLint* out) { *out = 0; };
    gl.onIsEnabled = [](auto) -> GLboolean { return GL_FALSE; };
  }
};

TEST_F(PostProcessorTest, ConstructRegistersService) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  EXPECT_EQ(engine.service<PostProcessor>(), &pp);
}

TEST_F(PostProcessorTest, AddPassAppendsInOrder) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  std::vector<RecordingPass::Record> log;
  auto* p1 = pp.addPass<RecordingPass>(&log);
  auto* p2 = pp.addPass<RecordingPass>(&log);
  ASSERT_EQ(pp.passes().size(), 2u);
  EXPECT_EQ(pp.passes()[0].get(), p1);
  EXPECT_EQ(pp.passes()[1].get(), p2);
}

TEST_F(PostProcessorTest, RemovePass) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  std::vector<RecordingPass::Record> log;
  auto* p1 = pp.addPass<RecordingPass>(&log);
  pp.addPass<RecordingPass>(&log);
  pp.removePass(p1);
  EXPECT_EQ(pp.passes().size(), 1u);
}

TEST_F(PostProcessorTest, ResizeUpdatesDimensions) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(640, 480);
  EXPECT_EQ(pp.width(), 640u);
  EXPECT_EQ(pp.height(), 480u);
  ASSERT_NE(pp.sceneTarget(), nullptr);
  EXPECT_EQ(pp.sceneTarget()->width(), 640u);
  EXPECT_EQ(pp.sceneTarget()->height(), 480u);
}

TEST_F(PostProcessorTest, BeginSimulationBindsSceneFbo) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<std::pair<GLenum, GLuint>> binds;
  gl.onBindFramebuffer = [&binds](GLenum target, GLuint id) {
    binds.emplace_back(target, id);
  };

  pp.beginSimulation();
  bool boundScene = false;
  for (auto& [target, id] : binds) {
    if (target == GL_FRAMEBUFFER && id != 0) {
      boundScene = true;
    }
  }
  EXPECT_TRUE(boundScene);
}

TEST_F(PostProcessorTest, EndSimulationWithNoPassesFallsBackToBlit) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  int drawArraysCount = 0;
  gl.onDrawArrays = [&drawArraysCount](auto, auto, auto) { drawArraysCount++; };

  pp.endSimulation();
  EXPECT_EQ(drawArraysCount, 1);
}

TEST_F(PostProcessorTest, ChainRoutesPingPong) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<RecordingPass::Record> log;
  pp.addPass<RecordingPass>(&log);
  pp.addPass<RecordingPass>(&log);
  pp.addPass<RecordingPass>(&log);

  pp.endSimulation();
  ASSERT_EQ(log.size(), 3u);

  EXPECT_EQ(log[0].input, pp.sceneTarget()->color());
  EXPECT_NE(log[0].output, nullptr);
  EXPECT_EQ(log[2].output, nullptr);
}

TEST_F(PostProcessorTest, DisabledPassesAreSkipped) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<RecordingPass::Record> log;
  pp.addPass<RecordingPass>(&log);
  auto* p2 = pp.addPass<RecordingPass>(&log);
  pp.addPass<RecordingPass>(&log);
  p2->enabled(false);

  pp.endSimulation();
  EXPECT_EQ(log.size(), 2u);
  EXPECT_EQ(log.back().output, nullptr);
}

TEST_F(PostProcessorTest, AllPassesDisabledTriggersFallback) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<RecordingPass::Record> log;
  auto* p1 = pp.addPass<RecordingPass>(&log);
  p1->enabled(false);

  int drawArraysCount = 0;
  gl.onDrawArrays = [&drawArraysCount](auto, auto, auto) { drawArraysCount++; };

  pp.endSimulation();
  EXPECT_TRUE(log.empty());
  EXPECT_EQ(drawArraysCount, 1);
}

TEST_F(PostProcessorTest, ViewportSizeChangeTriggersResize) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  engine.dispatchers()->dispatch<EngineEvent::ViewportSizeChange>(
      ViewportSizeChange{800, 600});
  EXPECT_EQ(pp.width(), 800u);
  EXPECT_EQ(pp.height(), 600u);
}

TEST_F(PostProcessorTest, RenderLayerChangeToUiRunsChain) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<RecordingPass::Record> log;
  pp.addPass<RecordingPass>(&log);

  engine.dispatchers()->dispatch<EngineEvent::RenderLayerChange>(
      RenderLayerChange{SceneLayer::UI});
  EXPECT_EQ(log.size(), 1u);
}

TEST_F(PostProcessorTest, RenderLayerChangeToSimulationDoesNotRunChain) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);

  std::vector<RecordingPass::Record> log;
  pp.addPass<RecordingPass>(&log);

  engine.dispatchers()->dispatch<EngineEvent::RenderLayerChange>(
      RenderLayerChange{SceneLayer::Simulation});
  EXPECT_EQ(log.size(), 0u);
}

TEST_F(PostProcessorTest, PassthroughPassDoesNotCrash) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  PostProcessor pp(&engine);
  pp.resize(128, 128);
  pp.addPass<PassthroughPass>();
  pp.endSimulation();
  SUCCEED();
}

}  // namespace uinta
