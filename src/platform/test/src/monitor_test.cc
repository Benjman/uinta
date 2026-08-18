#include "uinta/monitor.h"

#include <gtest/gtest.h>

namespace uinta {

class MonitorTestF : public ::testing::Test {
 protected:
};

TEST_F(MonitorTestF, DefaultConstructor) {
  Monitor monitor;

  EXPECT_EQ(monitor.name(), "");
  EXPECT_EQ(monitor.width(), 0);
  EXPECT_EQ(monitor.height(), 0);
  EXPECT_EQ(monitor.hz(), 0);
  EXPECT_EQ(monitor.userData(), nullptr);
  EXPECT_FALSE(monitor.flags().isPrimary());
  EXPECT_TRUE(monitor.flags().isConnected());  // Default is true
}

TEST_F(MonitorTestF, ParameterizedConstructor) {
  void* userData = reinterpret_cast<void*>(0x12345678);
  Monitor monitor("Test Monitor", 1920, 1080, 144, userData, true, true);

  EXPECT_EQ(monitor.name(), "Test Monitor");
  EXPECT_EQ(monitor.width(), 1920);
  EXPECT_EQ(monitor.height(), 1080);
  EXPECT_EQ(monitor.hz(), 144);
  EXPECT_EQ(monitor.userData(), userData);
  EXPECT_TRUE(monitor.flags().isPrimary());
  EXPECT_TRUE(monitor.flags().isConnected());
}

TEST_F(MonitorTestF, ConstructorNotPrimary) {
  Monitor monitor("Secondary", 2560, 1440, 165, nullptr, false);

  EXPECT_FALSE(monitor.flags().isPrimary());
}

TEST_F(MonitorTestF, ConstructorNotConnected) {
  Monitor monitor("Disconnected", 1920, 1080, 60, nullptr, false, false);

  EXPECT_FALSE(monitor.flags().isConnected());
}

// Monitor::Flags tests

class MonitorFlagsTestF : public ::testing::Test {
 protected:
};

TEST_F(MonitorFlagsTestF, IsPrimaryDefaultFalse) {
  Monitor::Flags flags;

  EXPECT_FALSE(flags.isPrimary());
}

TEST_F(MonitorFlagsTestF, SetIsPrimaryTrue) {
  Monitor::Flags flags;

  flags.isPrimary(true);

  EXPECT_TRUE(flags.isPrimary());
}

TEST_F(MonitorFlagsTestF, SetIsPrimaryFalse) {
  Monitor::Flags flags;
  flags.isPrimary(true);

  flags.isPrimary(false);

  EXPECT_FALSE(flags.isPrimary());
}

TEST_F(MonitorFlagsTestF, IsConnectedDefaultFalse) {
  Monitor::Flags flags;

  EXPECT_FALSE(flags.isConnected());
}

TEST_F(MonitorFlagsTestF, SetIsConnectedTrue) {
  Monitor::Flags flags;

  flags.isConnected(true);

  EXPECT_TRUE(flags.isConnected());
}

TEST_F(MonitorFlagsTestF, SetIsConnectedFalse) {
  Monitor::Flags flags;
  flags.isConnected(true);

  flags.isConnected(false);

  EXPECT_FALSE(flags.isConnected());
}

TEST_F(MonitorFlagsTestF, FlagsAreIndependent) {
  Monitor::Flags flags;

  flags.isPrimary(true);
  flags.isConnected(true);

  EXPECT_TRUE(flags.isPrimary());
  EXPECT_TRUE(flags.isConnected());

  flags.isPrimary(false);

  EXPECT_FALSE(flags.isPrimary());
  EXPECT_TRUE(flags.isConnected());  // Should remain unchanged

  flags.isConnected(false);

  EXPECT_FALSE(flags.isPrimary());
  EXPECT_FALSE(flags.isConnected());
}

}  // namespace uinta
