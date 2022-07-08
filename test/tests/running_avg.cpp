#include <gtest/gtest.h>

#define private public
#include <uinta/math/running_avg.hpp>

using namespace uinta;

TEST(RunningAvg, init_state) {
  RunningAvg avg(10);
  ASSERT_NE(nullptr, avg.buffer) << "Storage shouldn't be null.";
  ASSERT_FALSE(avg.dirty) << "Initial state should be false.";
  ASSERT_EQ(10, avg.count) << "Starting count should equal the buffer size.";
  ASSERT_EQ(0.0, avg.mavg) << "Starting average should be 0.0.";
  ASSERT_EQ(0, avg.position) << "Starting position should be 0.";
}

TEST(RunningAvg, empty) {
  ASSERT_EQ(0.0, RunningAvg(1).avg()) << "Empty average storage expected to return 0";
  ASSERT_FALSE(RunningAvg(1).dirty) << "Dirty state should remain false after avg().";
}

TEST(RunningAvg, position_advance) {
  RunningAvg avg(1);
  avg.add(2.0);
  ASSERT_EQ(1, avg.position) << "position should be 1 after adding buffer.";
}

TEST(RunningAvg, dirty_state) {
  RunningAvg avg(1);
  avg.add(2.0);
  ASSERT_TRUE(avg.dirty) << "Dirty state should be true after adding a buffer.";
}

TEST(RunningAvg, position_dirty_state) {
  RunningAvg avg(1);
  avg.add(2.0);
  ASSERT_TRUE(avg.dirty) << "Unexpected dirty state.";
}

TEST(RunningAvg, buffer) {
  RunningAvg avg(2);
  avg.add(2.0);
  avg.add(4.0);
  ASSERT_EQ(2.0, avg.buffer[0]);
  ASSERT_EQ(4.0, avg.buffer[1]);
}

TEST(RunningAvg, rolling_buffer) {
  RunningAvg avg(2);
  avg.add(2.0);
  avg.add(4.0);
  ASSERT_EQ(2.0, avg.buffer[0]);
  ASSERT_EQ(4.0, avg.buffer[1]);
  avg.add(6.0);
  ASSERT_EQ(6.0, avg.buffer[0]);
  ASSERT_EQ(4.0, avg.buffer[1]);
  avg.add(8.0);
  ASSERT_EQ(6.0, avg.buffer[0]);
  ASSERT_EQ(8.0, avg.buffer[1]);
  avg.add(10.0);
  ASSERT_EQ(10.0, avg.buffer[0]);
  ASSERT_EQ(8.0, avg.buffer[1]);
}

TEST(RunningAvg, avg) {
  RunningAvg avg(2);
  ASSERT_EQ(0.0, avg.avg());
  avg.add(2.0);
  ASSERT_EQ(2.0, avg.avg());
  avg.add(4.0);
  ASSERT_EQ(3.0, avg.avg());
}

TEST(RunningAvg, plus_operator) {
  float val = 1.3;
  RunningAvg avg(2);
  avg += val;
  ASSERT_EQ(val, avg.buffer[0]);
}
