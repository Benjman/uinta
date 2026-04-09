#include "uinta/utils/frame_manager.h"

#include <gtest/gtest.h>

#include "uinta/monitor.h"

namespace uinta {
namespace {

class FrameManagerTest : public ::testing::Test {};

TEST_F(FrameManagerTest, ConstructorWithValidMonitorSetsFrequency) {
  Monitor monitor("Test Monitor", 1920, 1080, 60);
  FrameManager fm(&monitor);
  EXPECT_FLOAT_EQ(1.0 / 60.0, fm.frequency);
}

TEST_F(FrameManagerTest, ConstructorWithNullMonitorDefaultsTo60Hz) {
  FrameManager fm(nullptr);
  EXPECT_FLOAT_EQ(1.0 / 60.0, fm.frequency);
}

TEST_F(FrameManagerTest, ConstructorWith144HzMonitor) {
  Monitor monitor("144Hz Monitor", 2560, 1440, 144);
  FrameManager fm(&monitor);
  EXPECT_FLOAT_EQ(1.0 / 144.0, fm.frequency);
}

TEST_F(FrameManagerTest, ConstructorWith240HzMonitor) {
  Monitor monitor("240Hz Monitor", 1920, 1080, 240);
  FrameManager fm(&monitor);
  EXPECT_FLOAT_EQ(1.0 / 240.0, fm.frequency);
}

TEST_F(FrameManagerTest, ConstructorWith30HzMonitor) {
  Monitor monitor("30Hz Monitor", 1920, 1080, 30);
  FrameManager fm(&monitor);
  EXPECT_FLOAT_EQ(1.0 / 30.0, fm.frequency);
}

TEST_F(FrameManagerTest, NextDefaultsToZero) {
  Monitor monitor("Test Monitor", 1920, 1080, 60);
  FrameManager fm(&monitor);
  EXPECT_EQ(0, fm.next);
}

TEST_F(FrameManagerTest, NextDefaultsToZeroWithNullMonitor) {
  FrameManager fm(nullptr);
  EXPECT_EQ(0, fm.next);
}

}  // namespace
}  // namespace uinta
