#include <gtest/gtest.h>

#include "uinta/mock/mock_platform.h"

namespace uinta {

class PlatformTestF : public ::testing::Test {
 protected:
};

TEST_F(PlatformTestF, PrimaryMonitorFound) {
  MockPlatform platform;

  auto result = platform.primaryMonitor();

  EXPECT_TRUE(result.has_value());
  EXPECT_TRUE(result.value()->flags().isPrimary());
}

TEST_F(PlatformTestF, PrimaryMonitorNotFound) {
  Monitor nonPrimary("Non-primary", 1920, 1080, 144, nullptr, false);
  MockPlatform platform(&nonPrimary);

  auto result = platform.primaryMonitor();

  EXPECT_FALSE(result.has_value());
}

TEST_F(PlatformTestF, FindMonitorFound) {
  void* userData = reinterpret_cast<void*>(0x12345678);
  Monitor monitorWithUserData("Test", 1920, 1080, 144, userData, true);
  MockPlatform platform(&monitorWithUserData);

  auto result = platform.findMonitor(userData);

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value()->userData(), userData);
}

TEST_F(PlatformTestF, FindMonitorNotFound) {
  MockPlatform platform;

  void* nonExistentPtr = reinterpret_cast<void*>(0xDEADBEEF);
  auto result = platform.findMonitor(nonExistentPtr);

  EXPECT_FALSE(result.has_value());
}

TEST_F(PlatformTestF, FindMonitorWithNullptr) {
  // Default MockPlatform monitor has nullptr userData
  MockPlatform platform;

  auto result = platform.findMonitor(nullptr);

  EXPECT_TRUE(result.has_value());
}

TEST_F(PlatformTestF, AddMonitor) {
  MockPlatform platform;
  size_t initialCount = platform.monitors().size();

  Monitor newMonitor("New Monitor", 2560, 1440, 165, nullptr, false);
  Monitor* added = platform.addMonitor(newMonitor);

  EXPECT_EQ(platform.monitors().size(), initialCount + 1);
  EXPECT_NE(added, nullptr);
  EXPECT_EQ(added->name(), "New Monitor");
  EXPECT_EQ(added->width(), 2560);
  EXPECT_EQ(added->height(), 1440);
}

TEST_F(PlatformTestF, MonitorsSpan) {
  MockPlatform platform;

  auto monitors = platform.monitors();

  EXPECT_FALSE(monitors.empty());
  EXPECT_EQ(monitors[0].name(), "Test monitor");
}

TEST_F(PlatformTestF, WindowAccessors) {
  MockPlatform platform;

  EXPECT_NE(platform.window(), nullptr);

  const MockPlatform& constPlatform = platform;
  EXPECT_NE(constPlatform.window(), nullptr);
}

TEST_F(PlatformTestF, StatusAccessor) {
  MockPlatform platform;

  EXPECT_TRUE(platform.status().ok());
}

TEST_F(PlatformTestF, EngineAccessor) {
  MockPlatform platform;

  EXPECT_EQ(platform.engine(), nullptr);
}

}  // namespace uinta
