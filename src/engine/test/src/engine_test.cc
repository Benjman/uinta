#include "uinta/engine/engine.h"

#include <gtest/gtest.h>

#include "./mock/mock_gl.h"
#include "./mock/mock_platform.h"
#include "absl/log/log.h"
#include "absl/time/time.h"
#include "mock/mock_runtime_getter.h"

namespace uinta {

TEST(Engine, constructorStartTimeUpdated) {
  MockPlatform platform;
  MockOpenGLApi gl;

  bool runtimeCalled = false;
  platform.onRuntime = [&]() {
    runtimeCalled = true;
    return platform.runtimeGetter_.runtime();
  };

  Engine engine(&platform, &gl);

  ASSERT_TRUE(runtimeCalled) << "`Engine()` was expected to capture the "
                                "current runtime of the application.";

  ASSERT_DOUBLE_EQ(MockRuntimeGetter::RuntimeStep, engine.state().runtime())
      << "`EngineState::runtime()` was expected to be updated in `Engine()`";
}

TEST(Engine, constructorOnClosingListening) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  ASSERT_FALSE(engine.state().isClosing());
  platform.dispatch<PlatformEvent::OnCloseRequest>(
      OnCloseRequestEvent(nullptr));
  ASSERT_TRUE(engine.state().isClosing());
}

TEST(Engine, constructorOnErrorListening) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  ASSERT_TRUE(engine.status().ok());
  platform.dispatch<PlatformEvent::OnError>(OnErrorEvent(0, ""));
  ASSERT_TRUE(IsInternal(engine.status()));
}

TEST(Engine, constructorOnViewportSizeChangeListening) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);

  bool eventCalled = false;
  engine.addListener<EngineEvent::ViewportSizeChange>(
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

TEST(Engine, constructorClearColorSet) {
  MockPlatform platform;
  MockOpenGLApi gl;

  bool clearColorCalled = false;
  gl.onClearColor = [&](GLfloat, GLfloat, GLfloat, GLfloat) {
    clearColorCalled = true;
  };

  Engine engine(&platform, &gl);
  ASSERT_TRUE(clearColorCalled);
}

inline void runThenClose(
    Engine* engine, std::function<bool()> condition = [] { return true; },
    absl::Duration timeout = absl::Milliseconds(20),
    absl::Duration forceTimeout = absl::Seconds(5)) noexcept {
  absl::Mutex mtx;
  auto threadFinished = false;

  std::thread thread([&]() {
    engine->run();
    {
      absl::MutexLock lock(&mtx);
      threadFinished = true;
    }
  });

  {
    absl::MutexLock lock(&mtx);
    auto conditionMet =
        mtx.AwaitWithTimeout(absl::Condition(&condition), timeout);

    engine->state().isClosing(true);

    if (!conditionMet) {
      LOG(ERROR) << "Condition was not met within the timeout period.";

      if (!mtx.AwaitWithTimeout(absl::Condition(&threadFinished), forceTimeout))
        LOG(ERROR) << "Engine did not exit after force timeout, potential hang "
                      "detected.";
    }
  }

  if (thread.joinable()) thread.join();
}

TEST(Engine, StopOnStateClosing) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  engine.state().isClosing(true);
  runThenClose(&engine);
  ASSERT_EQ(0, engine.state().tick());
}

TEST(Engine, runRuntimeUpdate) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  ASSERT_DOUBLE_EQ(MockRuntimeGetter::RuntimeStep, engine.state().runtime())
      << "Runtime was expected to have advanced in the constructor.";

  engine.state().isClosing(true);
  engine.run();

  constexpr size_t runtimeCallCountExpected = 2;
  ASSERT_DOUBLE_EQ(MockRuntimeGetter::RuntimeStep * runtimeCallCountExpected,
                   engine.state().runtime())
      << "Runtime was expected to advance two steps in `Engine::run()`.";
}

TEST(Engine, statusNotOkOnRun) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  engine.status() = InternalError("Testing error.");
  ASSERT_EQ(0, engine.state().tick());
  runThenClose(&engine);
}

TEST(Engine, singleTickAdvancement) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  ASSERT_EQ(0, engine.state().tick());
  runThenClose(&engine, [&]() { return engine.state().tick() > 0; });
  ASSERT_NE(0, engine.state().tick());
}

TEST(Engine, swapBuffersCalled) {
  bool swapBuffersCalled = false;
  MockPlatform platform;
  auto status = platform.onSwapBuffers = [&swapBuffersCalled]() {
    swapBuffersCalled = true;
    return OkStatus();
  };
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  runThenClose(&engine, [&swapBuffersCalled]() { return swapBuffersCalled; });
  ASSERT_TRUE(swapBuffersCalled);
}

TEST(Engine, clearCalled) {
  bool clearCalled = false;
  GLbitfield bitsVal = 0;
  MockPlatform platform;
  MockOpenGLApi gl;
  gl.onClear = [&clearCalled, &bitsVal](GLbitfield bits) {
    clearCalled = true;
    bitsVal = bits;
    return OkStatus();
  };
  Engine engine(&platform, &gl);
  runThenClose(&engine, [&clearCalled]() { return clearCalled; });
  ASSERT_TRUE(clearCalled);
  EXPECT_TRUE(bitsVal | GL_COLOR_BUFFER_BIT);
  EXPECT_TRUE(bitsVal | GL_DEPTH_BUFFER_BIT);
}

TEST(Engine, EventTickComplete) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  bool tickCompleteEventCalled = false;
  engine.addListener<EngineEvent::TickComplete>(
      [&tickCompleteEventCalled](const auto&) {
        tickCompleteEventCalled = true;
      });
  runThenClose(&engine, [&tickCompleteEventCalled]() {
    return tickCompleteEventCalled;
  });
  ASSERT_TRUE(tickCompleteEventCalled);
}

TEST(Engine, EventRenderComplete) {
  MockPlatform platform;
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  bool renderCompleteEventCalled = false;
  engine.addListener<EngineEvent::RenderComplete>(
      [&renderCompleteEventCalled](const auto&) {
        renderCompleteEventCalled = true;
      });
  runThenClose(&engine, [&renderCompleteEventCalled]() {
    return renderCompleteEventCalled;
  });
  ASSERT_TRUE(renderCompleteEventCalled);
}

TEST(Engine, EventOnMonitorChange) {
  Monitor firstMon("First monitor", 1920, 1080, 60, nullptr, true);
  Monitor secondMon("Second monitor", 1920, 1080, 144);
  MockPlatform platform(&firstMon);
  MockOpenGLApi gl;
  Engine engine(&platform, &gl);
  auto firstMonExpectedAdvance = 1.0 / firstMon.hz();
  ASSERT_FLOAT_EQ(firstMonExpectedAdvance,
                  engine.frameManager().nextFrameAdvance);
  platform.dispatch<PlatformEvent::OnMonitorChange>(
      OnMonitorChangeEvent(&secondMon));
  auto secondMonExpectedAdvance = 1.0 / secondMon.hz();
  ASSERT_FLOAT_EQ(secondMonExpectedAdvance,
                  engine.frameManager().nextFrameAdvance);
}

}  // namespace uinta
