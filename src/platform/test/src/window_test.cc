#include "uinta/window.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_platform.h"

namespace uinta {

class WindowTestF : public ::testing::Test {
 protected:
};

TEST_F(WindowTestF, ConstructorWithPrimaryMonitor) {
  MockPlatform platform;
  Window window(&platform);

  EXPECT_TRUE(window.status().ok());
  EXPECT_NE(window.monitor(), nullptr);
  EXPECT_EQ(window.width(), 800);
  EXPECT_EQ(window.height(), 600);
}

TEST_F(WindowTestF, ConstructorNoPrimaryMonitor) {
  Monitor nonPrimaryMonitor("Non-primary", 1920, 1080, 144, nullptr, false);
  MockPlatform platform(&nonPrimaryMonitor);
  Window window(&platform);

  EXPECT_FALSE(window.status().ok());
  EXPECT_TRUE(IsNotFound(window.status()));
}

// Note: Cannot easily test empty monitor list without modifying MockPlatform
// since monitors_ is protected in Platform base class. The primaryMonitor()
// logic is tested via ConstructorNoPrimaryMonitor which covers the error path.

TEST_F(WindowTestF, AspectRatio) {
  MockPlatform platform;
  Window window(&platform);
  window.size(1920, 1080);

  EXPECT_FLOAT_EQ(window.aspect(), 1920.0f / 1080.0f);
}

TEST_F(WindowTestF, AspectRatioSquare) {
  MockPlatform platform;
  Window window(&platform);
  window.size(1000, 1000);

  EXPECT_FLOAT_EQ(window.aspect(), 1.0f);
}

TEST_F(WindowTestF, SizeGettersSetters) {
  MockPlatform platform;
  Window window(&platform);

  window.size(1280, 720);
  EXPECT_EQ(window.width(), 1280);
  EXPECT_EQ(window.height(), 720);
}

TEST_F(WindowTestF, PositionGettersSetters) {
  MockPlatform platform;
  Window window(&platform);

  window.pos(100, 200);
  EXPECT_EQ(window.x(), 100);
  EXPECT_EQ(window.y(), 200);
}

TEST_F(WindowTestF, FullscreenGetterSetter) {
  MockPlatform platform;
  Window window(&platform);

  EXPECT_FALSE(window.isFullscreen());
  window.isFullscreen(true);
  EXPECT_TRUE(window.isFullscreen());
  window.isFullscreen(false);
  EXPECT_FALSE(window.isFullscreen());
}

TEST_F(WindowTestF, DefaultName) {
  MockPlatform platform;
  Window window(&platform);

  EXPECT_EQ(window.name(), "Uinta Engine");
}

TEST_F(WindowTestF, MonitorGetterSetter) {
  MockPlatform platform;
  Window window(&platform);

  Monitor newMonitor("New Monitor", 2560, 1440, 165, nullptr, false);
  window.monitor(&newMonitor);
  EXPECT_EQ(window.monitor(), &newMonitor);
}

}  // namespace uinta
