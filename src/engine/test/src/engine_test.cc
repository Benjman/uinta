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

}  // namespace uinta
