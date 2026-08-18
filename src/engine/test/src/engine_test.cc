#include "uinta/engine/engine.h"

#include <gtest/gtest.h>

#include "./utils.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class EngineTest : public UintaTestF {
 protected:
  static void closeAfterFrames(MockPlatform* platform, Engine* engine, int frames = 1) noexcept {
    platform->onSwapBuffers = [engine, frames, count = 0]() mutable {
      if (++count >= frames) {
        engine->state().isClosing(true);
      }
      return OkStatus();
    };
  }
};

TEST_F(EngineTest, ConstructorOnClosingListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  ASSERT_FALSE(engine.state().isClosing());
  platform.dispatch<PlatformEvent::OnCloseRequest>(OnCloseRequestEvent(nullptr));
  ASSERT_TRUE(engine.state().isClosing());
}

TEST_F(EngineTest, ConstructorOnErrorListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  EXPECT_DEATH(platform.dispatch<PlatformEvent::OnError>(OnErrorEvent(0, "")), "");
}

TEST_F(EngineTest, ConstructorOnViewportSizeChangeListening) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  bool eventCalled = false;
  engine.dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [&eventCalled](const auto&) { eventCalled = true; });

  bool onViewportCalled = false;
  gl.onViewport = [&onViewportCalled](auto, auto, auto, auto) { onViewportCalled = true; };
  platform.dispatch<PlatformEvent::OnViewportSizeChange>(OnViewportSizeChangeEvent(nullptr, 0, 0));
  ASSERT_TRUE(onViewportCalled);
  ASSERT_TRUE(eventCalled);
}

TEST_F(EngineTest, StopOnStateClosing) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  engine.state().isClosing(true);
  engine.run();
  ASSERT_EQ(0, engine.state().tick());
}

TEST_F(EngineTest, SingleTickAdvancement) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  closeAfterFrames(&platform, &engine);
  ASSERT_EQ(0, engine.state().tick());
  engine.run();
  ASSERT_EQ(1, engine.state().tick());
}

TEST_F(EngineTest, SwapBuffersCalled) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  bool swapBuffersCalled = false;
  platform.onSwapBuffers = [&]() {
    swapBuffersCalled = true;
    engine.state().isClosing(true);
    return OkStatus();
  };
  engine.run();
  ASSERT_TRUE(swapBuffersCalled);
}

TEST_F(EngineTest, ClearCalled) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  closeAfterFrames(&platform, &engine);
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

}  // namespace uinta
