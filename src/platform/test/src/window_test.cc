#include "uinta/window.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_platform.h"

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

class WindowTestF : public ::testing::Test {
 protected:
  MockAppConfig appConfig;

  void SetUp() override { configureDefaultWindowSettings(&appConfig); }
};

TEST_F(WindowTestF, ConstructorWithPrimaryMonitor) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  EXPECT_TRUE(window.status().ok());
  EXPECT_NE(window.monitor(), nullptr);
  EXPECT_EQ(window.width(), 800);
  EXPECT_EQ(window.height(), 600);
}

TEST_F(WindowTestF, ConstructorNoPrimaryMonitor) {
  Monitor nonPrimaryMonitor("Non-primary", 1920, 1080, 144, nullptr, false);
  MockPlatform platform(&nonPrimaryMonitor);
  Window window(&platform, &appConfig);

  EXPECT_FALSE(window.status().ok());
  EXPECT_TRUE(IsNotFound(window.status()));
}

// Note: Cannot easily test empty monitor list without modifying MockPlatform
// since monitors_ is protected in Platform base class. The primaryMonitor()
// logic is tested via ConstructorNoPrimaryMonitor which covers the error path.

TEST_F(WindowTestF, AspectRatio) {
  MockPlatform platform;
  Window window(&platform, &appConfig);
  window.size(1920, 1080);

  EXPECT_FLOAT_EQ(window.aspect(), 1920.0f / 1080.0f);
}

TEST_F(WindowTestF, AspectRatioSquare) {
  MockPlatform platform;
  Window window(&platform, &appConfig);
  window.size(1000, 1000);

  EXPECT_FLOAT_EQ(window.aspect(), 1.0f);
}

TEST_F(WindowTestF, SizeGettersSetters) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  window.size(1280, 720);
  EXPECT_EQ(window.width(), 1280);
  EXPECT_EQ(window.height(), 720);
}

TEST_F(WindowTestF, PositionGettersSetters) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  window.pos(100, 200);
  EXPECT_EQ(window.x(), 100);
  EXPECT_EQ(window.y(), 200);
}

TEST_F(WindowTestF, FullscreenGetterSetter) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  EXPECT_FALSE(window.isFullscreen());
  window.isFullscreen(true);
  EXPECT_TRUE(window.isFullscreen());
  window.isFullscreen(false);
  EXPECT_FALSE(window.isFullscreen());
}

TEST_F(WindowTestF, DefaultName) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  EXPECT_EQ(window.name(), "Uinta Engine");
}

TEST_F(WindowTestF, MonitorGetterSetter) {
  MockPlatform platform;
  Window window(&platform, &appConfig);

  Monitor newMonitor("New Monitor", 2560, 1440, 165, nullptr, false);
  window.monitor(&newMonitor);
  EXPECT_EQ(window.monitor(), &newMonitor);
}

}  // namespace uinta
