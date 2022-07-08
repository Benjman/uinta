#include <gtest/gtest.h>

#include "./tests/math/smooth_float_test.cpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
