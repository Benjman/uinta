#include "uinta/desktop_platform.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_desktop_platform_api.h"

namespace uinta {

namespace {

// Helper to configure MockAppConfig with default window settings
void configureDefaultWindowSettings(MockAppConfig* config) {
  config->onGetBoolean = [](const std::string& key) -> std::optional<bool> {
    if (key == "platform.window.fullscreen") {
      return false;
    }
    return std::nullopt;
  };
  config->onGetString =
      [](const std::string& key) -> std::optional<std::string> {
    if (key == "platform.window.name") {
      return "Uinta Engine";
    }
    return std::nullopt;
  };
  config->onGetInt2 = [](const std::string& key,
                         i32* ptr) -> std::optional<i32*> {
    if (key == "platform.window.position") {
      ptr[0] = 0;
      ptr[1] = 0;
      return ptr;
    }
    if (key == "platform.window.size") {
      ptr[0] = 800;
      ptr[1] = 600;
      return ptr;
    }
    return std::nullopt;
  };
}

}  // namespace

class DesktopPlatformTestF : public ::testing::Test {
 protected:
  MockDesktopPlatformApi api;
  MockAppConfig appConfig;

  void SetUp() override { configureDefaultWindowSettings(&appConfig); }
};

TEST_F(DesktopPlatformTestF, ConstructorSuccess) {
  DesktopPlatform platform(&appConfig, &api);

  EXPECT_TRUE(platform.status().ok());
  EXPECT_NE(platform.window(), nullptr);
  EXPECT_FALSE(platform.monitors().empty());
}

TEST_F(DesktopPlatformTestF, ConstructorInitOpenGLFails) {
  api.onInitOpenGL = []() -> Status {
    return InternalError("OpenGL initialization failed");
  };

  DesktopPlatform platform(&appConfig, &api);

  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsInternal(platform.status()));
}

TEST_F(DesktopPlatformTestF, ConstructorFindMonitorsFails) {
  api.onFindMonitors = []() -> StatusOr<std::vector<Monitor>> {
    return NotFoundError("No monitors found");
  };

  DesktopPlatform platform(&appConfig, &api);

  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsNotFound(platform.status()));
}

TEST_F(DesktopPlatformTestF, ConstructorFindMonitorsReturnsEmpty) {
  api.onFindMonitors = []() -> StatusOr<std::vector<Monitor>> {
    return std::vector<Monitor>{};
  };

  DesktopPlatform platform(&appConfig, &api);

  // Empty monitors means no primary monitor, which causes Window creation to
  // fail
  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsNotFound(platform.status()));
}

TEST_F(DesktopPlatformTestF, ConstructorCreateWindowFails) {
  api.onCreateWindow = [](Window*) -> StatusOr<void*> {
    return InternalError("Failed to create window");
  };

  DesktopPlatform platform(&appConfig, &api);

  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsInternal(platform.status()));
}

TEST_F(DesktopPlatformTestF, PollEventsDelegatesToApi) {
  bool pollEventsCalled = false;
  api.onPollEvents = [&pollEventsCalled]() -> Status {
    pollEventsCalled = true;
    return OkStatus();
  };

  DesktopPlatform platform(&appConfig, &api);
  ASSERT_TRUE(platform.status().ok());

  auto status = platform.pollEvents();

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(pollEventsCalled);
}

TEST_F(DesktopPlatformTestF, RuntimeDelegatesToApi) {
  api.onRuntime = []() -> StatusOr<time_t> { return 42.0; };

  DesktopPlatform platform(&appConfig, &api);
  ASSERT_TRUE(platform.status().ok());

  auto runtime = platform.runtime();

  EXPECT_TRUE(runtime.ok());
  EXPECT_EQ(runtime.value(), 42.0);
}

TEST_F(DesktopPlatformTestF, SwapBuffersDelegatesToApi) {
  bool swapBuffersCalled = false;
  const Window* capturedWindow = nullptr;
  api.onSwapBuffers = [&](const Window* w) -> Status {
    swapBuffersCalled = true;
    capturedWindow = w;
    return OkStatus();
  };

  DesktopPlatform platform(&appConfig, &api);
  ASSERT_TRUE(platform.status().ok());

  auto status = platform.swapBuffers();

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(swapBuffersCalled);
  EXPECT_EQ(capturedWindow, platform.window());
}

TEST_F(DesktopPlatformTestF, GetAndUpdateWindowSizeDelegatesToApi) {
  api.onGetWindowSize = [](const Window*, i32* w, i32* h) {
    *w = 1920;
    *h = 1080;
  };

  DesktopPlatform platform(&appConfig, &api);
  ASSERT_TRUE(platform.status().ok());

  i32 width;
  i32 height;
  platform.getAndUpdateWindowSize(&width, &height);

  EXPECT_EQ(width, 1920);
  EXPECT_EQ(height, 1080);
  EXPECT_EQ(platform.window()->width(), 1920);
  EXPECT_EQ(platform.window()->height(), 1080);
}

TEST_F(DesktopPlatformTestF, DestroyDelegatesToApi) {
  bool destroyCalled = false;
  api.onDestroy = [&destroyCalled](const Window*) -> Status {
    destroyCalled = true;
    return OkStatus();
  };

  {
    DesktopPlatform platform(&appConfig, &api);
    ASSERT_TRUE(platform.status().ok());
  }  // Destructor called here

  EXPECT_TRUE(destroyCalled);
}

}  // namespace uinta
