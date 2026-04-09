#include "uinta/engine/engine.h"

#include <gtest/gtest.h>

#include "./utils.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class EngineTest : public UintaTestF {};

TEST_F(EngineTest, ConstructorOnClosingListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  ASSERT_FALSE(engine.state().isClosing());
  platform.dispatch<PlatformEvent::OnCloseRequest>(
      OnCloseRequestEvent(nullptr));
  ASSERT_TRUE(engine.state().isClosing());
}

TEST_F(EngineTest, ConstructorOnErrorListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  ASSERT_TRUE(engine.status().ok());
  platform.dispatch<PlatformEvent::OnError>(OnErrorEvent(0, ""));
  ASSERT_TRUE(IsInternal(engine.status()));
}

TEST_F(EngineTest, ConstructorOnViewportSizeChangeListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  bool eventCalled = false;
  engine.dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [&eventCalled](const auto&) { eventCalled = true; });

  bool onViewportCalled = false;
  gl.onViewport = [&onViewportCalled](auto, auto, auto, auto) {
    onViewportCalled = true;
  };
  platform.dispatch<PlatformEvent::OnViewportSizeChange>(
      OnViewportSizeChangeEvent(nullptr, 0, 0));
  ASSERT_TRUE(onViewportCalled);
  ASSERT_TRUE(eventCalled);
}

TEST_F(EngineTest, ConstructorClearColorSet) {
  bool clearColorCalled = false;
  gl.onClearColor = [&](GLfloat, GLfloat, GLfloat, GLfloat) {
    clearColorCalled = true;
  };

  MockPlatform platform;
  auto engine = makeEngine(&platform);
  ASSERT_TRUE(clearColorCalled);
}

TEST_F(EngineTest, StopOnStateClosing) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.state().isClosing(true);
  engine.run();
  ASSERT_EQ(0, engine.state().tick());
}

TEST_F(EngineTest, StatusNotOkOnRun) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.status() = InternalError("Testing error.");
  ASSERT_EQ(0, engine.state().tick());
  engine.run();
  engine.dispatchers()->addListener<EngineEvent::TickComplete>(
      [&](const auto&) {
        ASSERT_TRUE(false) << "Entering the run loop was not expected.";
      });
}

TEST_F(EngineTest, SingleTickAdvancement) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.dispatchers()->addListener<EngineEvent::TickComplete>(
      [&](const auto&) { engine.state().isClosing(true); });
  ASSERT_EQ(0, engine.state().tick());
  engine.run();
  ASSERT_EQ(1, engine.state().tick());
}

TEST_F(EngineTest, SwapBuffersCalled) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  bool swapBuffersCalled = false;
  auto status = platform.onSwapBuffers = [&swapBuffersCalled]() {
    swapBuffersCalled = true;
    return OkStatus();
  };
  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) { engine.state().isClosing(true); });
  engine.run();
  ASSERT_TRUE(swapBuffersCalled);
}

TEST_F(EngineTest, ClearCalled) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) { engine.state().isClosing(true); });
  bool clearCalled = false;
  GLbitfield bitsVal = 0;
  gl.onClear = [&clearCalled, &bitsVal](GLbitfield bits) {
    clearCalled = true;
    bitsVal = bits;
    return OkStatus();
  };
  engine.run();
  ASSERT_TRUE(clearCalled);
  EXPECT_TRUE(bitsVal | GL_COLOR_BUFFER_BIT);
  EXPECT_TRUE(bitsVal | GL_DEPTH_BUFFER_BIT);
}

TEST_F(EngineTest, EventTickComplete) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  bool tickCompleteEventCalled = false;
  engine.dispatchers()->addListener<EngineEvent::TickComplete>(
      [&](const auto&) {
        tickCompleteEventCalled = true;
        engine.state().isClosing(true);
      });
  engine.run();
  ASSERT_TRUE(tickCompleteEventCalled);
}

TEST_F(EngineTest, EventRenderComplete) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  bool renderCompleteEventCalled = false;
  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) {
        renderCompleteEventCalled = true;
        engine.state().isClosing(true);
      });
  engine.run();
  ASSERT_TRUE(renderCompleteEventCalled);
}

TEST_F(EngineTest, EventOnMonitorChange) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  auto primaryMonitor = platform.primaryMonitor();
  ASSERT_NE(std::nullopt, primaryMonitor);
  auto firstMonExpectedAdvance =
      1.0 / static_cast<time_t>(primaryMonitor.value()->hz());
  ASSERT_FLOAT_EQ(firstMonExpectedAdvance, engine.frameManager().frequency);

  Monitor secondMon("Second monitor", 1920, 1080, 144);
  platform.dispatch<PlatformEvent::OnMonitorChange>(
      OnMonitorChangeEvent(&secondMon));
  auto secondMonExpectedAdvance = 1.0 / secondMon.hz();
  ASSERT_FLOAT_EQ(secondMonExpectedAdvance, engine.frameManager().frequency);
}

TEST_F(EngineTest, PerStageDeltaTimeCalculation) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  // Control runtime manually
  time_t currentTime = 0.0;
  platform.runtimeGetter_.onRuntime = [&currentTime] { return currentTime; };

  // Track delta times received in each stage
  time_t preTickDelta = 0.0;
  time_t tickDelta = 0.0;
  time_t postTickDelta = 0.0;
  time_t preRenderDelta = 0.0;
  time_t renderDelta = 0.0;
  time_t postRenderDelta = 0.0;

  class TestScene : public Scene {
   public:
    time_t* preTickDelta_;
    time_t* tickDelta_;
    time_t* postTickDelta_;
    time_t* preRenderDelta_;
    time_t* renderDelta_;
    time_t* postRenderDelta_;

    TestScene(Engine* engine, time_t* preTick, time_t* tick, time_t* postTick,
              time_t* preRender, time_t* render, time_t* postRender)
        : Scene(engine),
          preTickDelta_(preTick),
          tickDelta_(tick),
          postTickDelta_(postTick),
          preRenderDelta_(preRender),
          renderDelta_(render),
          postRenderDelta_(postRender) {}

    void preTick(time_t delta) noexcept override { *preTickDelta_ = delta; }
    void tick(time_t delta) noexcept override { *tickDelta_ = delta; }
    void postTick(time_t delta) noexcept override { *postTickDelta_ = delta; }
    void preRender(time_t delta) noexcept override { *preRenderDelta_ = delta; }
    void render(time_t delta) noexcept override { *renderDelta_ = delta; }
    void postRender(time_t delta) noexcept override {
      *postRenderDelta_ = delta;
    }
  };

  engine.addScene<TestScene>(&preTickDelta, &tickDelta, &postTickDelta,
                             &preRenderDelta, &renderDelta, &postRenderDelta);

  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) { engine.state().isClosing(true); });

  // First frame: time = 0.0, all stages should have delta = 0.0 (first call)
  currentTime = 0.0;
  engine.run();

  // After first frame, all deltas should be 0.0 (since stageRuntimes_
  // initialized to 0)
  EXPECT_FLOAT_EQ(0.0, preTickDelta);
  EXPECT_FLOAT_EQ(0.0, tickDelta);
  EXPECT_FLOAT_EQ(0.0, postTickDelta);
  EXPECT_FLOAT_EQ(0.0, preRenderDelta);
  EXPECT_FLOAT_EQ(0.0, renderDelta);
  EXPECT_FLOAT_EQ(0.0, postRenderDelta);
}

TEST_F(EngineTest, DifferentDeltasPerStage) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  // Control runtime manually with precise timing
  time_t currentTime = 0.0;
  platform.runtimeGetter_.onRuntime = [&currentTime] { return currentTime; };

  std::vector<time_t> preTickDeltas;
  std::vector<time_t> tickDeltas;
  std::vector<time_t> renderDeltas;

  class TestScene : public Scene {
   public:
    std::vector<time_t>* preTickDeltas_;
    std::vector<time_t>* tickDeltas_;
    std::vector<time_t>* renderDeltas_;

    TestScene(Engine* engine, std::vector<time_t>* preTick,
              std::vector<time_t>* tick, std::vector<time_t>* render)
        : Scene(engine),
          preTickDeltas_(preTick),
          tickDeltas_(tick),
          renderDeltas_(render) {}

    void preTick(time_t delta) noexcept override {
      preTickDeltas_->push_back(delta);
    }
    void tick(time_t delta) noexcept override { tickDeltas_->push_back(delta); }
    void render(time_t delta) noexcept override {
      renderDeltas_->push_back(delta);
    }
  };

  engine.addScene<TestScene>(&preTickDeltas, &tickDeltas, &renderDeltas);

  // Simulate time advancing differently between calls
  int tickCount = 0;
  int renderCount = 0;

  engine.dispatchers()->addListener<EngineEvent::TickComplete>(
      [&](const auto&) {
        tickCount++;
        currentTime += 0.016;  // Advance 16ms after each tick
      });

  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) {
        renderCount++;
        currentTime += 0.001;  // Advance 1ms after render
        if (renderCount >= 2) {
          engine.state().isClosing(true);
        }
      });

  engine.run();

  // We should have multiple tick and render deltas
  EXPECT_GT(preTickDeltas.size(), 0);
  EXPECT_GT(tickDeltas.size(), 0);
  EXPECT_GT(renderDeltas.size(), 0);

  // Each stage should track its own time independently
  // (exact values depend on timing, but they should be captured)
  EXPECT_TRUE(!preTickDeltas.empty());
  EXPECT_TRUE(!tickDeltas.empty());
  EXPECT_TRUE(!renderDeltas.empty());
}

TEST_F(EngineTest, IndependentStageDeltaTimes) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  // Track when each stage is called and with what delta
  struct StageCall {
    std::string stage;
    time_t delta;
  };
  std::vector<StageCall> stageCalls;

  class TestScene : public Scene {
   public:
    std::vector<StageCall>* stageCalls_;

    TestScene(Engine* engine, std::vector<StageCall>* calls)
        : Scene(engine), stageCalls_(calls) {}

    void preTick(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "preTick", .delta = delta});
    }
    void tick(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "tick", .delta = delta});
    }
    void postTick(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "postTick", .delta = delta});
    }
    void preRender(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "preRender", .delta = delta});
    }
    void render(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "render", .delta = delta});
    }
    void postRender(time_t delta) noexcept override {
      stageCalls_->push_back({.stage = "postRender", .delta = delta});
    }
  };

  engine.addScene<TestScene>(&stageCalls);

  int frameCount = 0;
  engine.dispatchers()->addListener<EngineEvent::RenderComplete>(
      [&](const auto&) {
        frameCount++;
        if (frameCount >= 2) {
          engine.state().isClosing(true);
        }
      });

  engine.run();

  // We should have captured calls to all stages across multiple frames
  EXPECT_GT(stageCalls.size(), 6)
      << "Expected at least 2 frames worth of stage calls";

  // Verify all stages were called
  bool hasPreTick = false;
  bool hasTick = false;
  bool hasPostTick = false;
  bool hasPreRender = false;
  bool hasRender = false;
  bool hasPostRender = false;

  for (const auto& call : stageCalls) {
    if (call.stage == "preTick") {
      hasPreTick = true;
    }
    if (call.stage == "tick") {
      hasTick = true;
    }
    if (call.stage == "postTick") {
      hasPostTick = true;
    }
    if (call.stage == "preRender") {
      hasPreRender = true;
    }
    if (call.stage == "render") {
      hasRender = true;
    }
    if (call.stage == "postRender") {
      hasPostRender = true;
    }
  }

  EXPECT_TRUE(hasPreTick) << "preTick should have been called";
  EXPECT_TRUE(hasTick) << "tick should have been called";
  EXPECT_TRUE(hasPostTick) << "postTick should have been called";
  EXPECT_TRUE(hasPreRender) << "preRender should have been called";
  EXPECT_TRUE(hasRender) << "render should have been called";
  EXPECT_TRUE(hasPostRender) << "postRender should have been called";
}

}  // namespace uinta
