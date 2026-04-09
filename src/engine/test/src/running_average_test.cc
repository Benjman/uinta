#include "uinta/math/running_average.h"

#include <gtest/gtest.h>

#include "uinta/types.h"

namespace uinta {

TEST(RunningAverage, EmptyBuffer) {
  RunningAverage<f32, 3> average;
  ASSERT_EQ(0, average.getAverage())
      << "Average without elements should return zero.";
}

TEST(RunningAverage, SingleElement) {
  RunningAverage<f32, 3> average;
  ASSERT_EQ(0, average.getAverage());
  average.push(1);
  ASSERT_EQ(1, average.getAverage());
}

TEST(RunningAverage, MultipleElements) {
  RunningAverage<f32, 3> average;
  ASSERT_EQ(0, average.getAverage());
  average.push(1);
  average.push(3);
  ASSERT_EQ(2, average.getAverage());
}

TEST(RunningAverage, FullBuffer) {
  RunningAverage<f32, 3> average;
  ASSERT_EQ(0, average.getAverage());
  average.push(1);
  average.push(3);
  average.push(5);
  ASSERT_EQ(3, average.getAverage());
}

TEST(RunningAverage, Overflow) {
  RunningAverage<f32, 3> average;
  ASSERT_EQ(0, average.getAverage());
  average.push(1);
  average.push(3);
  average.push(5);
  ASSERT_EQ(3, average.getAverage());
  average.push(7);
  ASSERT_EQ(5, average.getAverage());
}

}  // namespace uinta
