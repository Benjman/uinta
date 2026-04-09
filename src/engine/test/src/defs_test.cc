#include "uinta/math/defs.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

namespace uinta {
namespace {

TEST(DefsTest, EpsilonFIsFloatEpsilon) {
  EXPECT_EQ(std::numeric_limits<f32>::epsilon(), epsilon_f);
}

TEST(DefsTest, EpsilonFIsPositive) { EXPECT_GT(epsilon_f, 0.0f); }

TEST(DefsTest, EpsilonFIsSmall) { EXPECT_LT(epsilon_f, 0.001f); }

TEST(DefsTest, RootThreeFIsCorrect) {
  EXPECT_FLOAT_EQ(std::sqrt(3.0f), rootThree_f);
}

TEST(DefsTest, RootThreeFApproximateValue) {
  EXPECT_NEAR(1.732050808f, rootThree_f, epsilon_f);
}

TEST(DefsTest, PIIsCorrect) { EXPECT_FLOAT_EQ(3.14159265358979323846f, PI); }

TEST(DefsTest, PIApproximateValue) { EXPECT_NEAR(3.14159f, PI, 0.00001f); }

TEST(DefsTest, PIUsableInCalculations) {
  // sin(PI) should be approximately 0
  EXPECT_NEAR(0.0f, std::sin(PI), epsilon_f);
  // cos(PI) should be approximately -1
  EXPECT_NEAR(-1.0f, std::cos(PI), epsilon_f);
}

}  // namespace
}  // namespace uinta
