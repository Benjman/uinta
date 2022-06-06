#include <gtest/gtest.h>

#define private public
#include <uinta/math.hpp>

TEST(running_avg, init_state) {
  running_avg avg(10);
  ASSERT_NE(nullptr, avg.buffer) << "Storage shouldn't be null.";
  ASSERT_FALSE(avg.dirty) << "Initial state should be false.";
  ASSERT_EQ(10, avg.count) << "Starting count should equal the buffer size.";
  ASSERT_EQ(0.0, avg.mavg) << "Starting average should be 0.0.";
  ASSERT_EQ(0, avg.cursor) << "Starting cursor should be 0.";
}

TEST(running_avg, empty) {
  ASSERT_EQ(0.0, running_avg(1).avg()) << "Empty average storage expected to return 0";
  ASSERT_FALSE(running_avg(1).dirty) << "Dirty state should remain false after avg().";
}

TEST(running_avg, cursor_advance) {
  running_avg avg(1);
  avg.add(2.0);
  ASSERT_EQ(1, avg.cursor) << "Cursor should be 1 after adding buffer.";
}

TEST(running_avg, dirty_state) {
  running_avg avg(1);
  avg.add(2.0);
  ASSERT_TRUE(avg.dirty) << "Dirty state should be true after adding a buffer.";
}

TEST(running_avg, cursor_dirty_state) {
  running_avg avg(1);
  avg.add(2.0);
  ASSERT_TRUE(avg.dirty) << "Unexpected dirty state.";
}

TEST(running_avg, buffer) {
  running_avg avg(2);
  avg.add(2.0);
  avg.add(4.0);
  ASSERT_EQ(2.0, avg.buffer[0]);
  ASSERT_EQ(4.0, avg.buffer[1]);
}

TEST(running_avg, rolling_buffer) {
  running_avg avg(2);
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

TEST(running_avg, avg) {
  running_avg avg(2);
  ASSERT_EQ(0.0, avg.avg());
  avg.add(2.0);
  ASSERT_EQ(2.0, avg.avg());
  avg.add(4.0);
  ASSERT_EQ(3.0, avg.avg());
}

TEST(running_avg, plus_operator) {
  float val = 1.3;
  running_avg avg(2);
  avg += val;
  ASSERT_EQ(val, avg.buffer[0]);
}
