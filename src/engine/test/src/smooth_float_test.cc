#include "uinta/math/smooth_float.h"

#include <gtest/gtest.h>

namespace uinta {
namespace {

class SmoothFloatTest : public ::testing::Test {
 protected:
  static constexpr f32 kAgility = 5.0f;
  static constexpr f32 kStartValue = 10.0f;
};

// Constructor tests
TEST_F(SmoothFloatTest, ConstructorSetsAgility) {
  SmoothFloat sf(kAgility);
  EXPECT_FLOAT_EQ(kAgility, sf.agility());
}

TEST_F(SmoothFloatTest, ConstructorSetsDefaultStart) {
  SmoothFloat sf(kAgility);
  EXPECT_FLOAT_EQ(0.0f, sf.current());
  EXPECT_FLOAT_EQ(0.0f, sf.target());
}

TEST_F(SmoothFloatTest, ConstructorSetsStartValue) {
  SmoothFloat sf(kAgility, kStartValue);
  EXPECT_FLOAT_EQ(kStartValue, sf.current());
  EXPECT_FLOAT_EQ(kStartValue, sf.target());
}

// force() tests
TEST_F(SmoothFloatTest, ForceSetsBothCurrentAndTarget) {
  SmoothFloat sf(kAgility, 0.0f);
  sf = 100.0f;  // Set target
  sf.force(50.0f);
  EXPECT_FLOAT_EQ(50.0f, sf.current());
  EXPECT_FLOAT_EQ(50.0f, sf.target());
}

TEST_F(SmoothFloatTest, ForceNoArgUsesTarget) {
  SmoothFloat sf(kAgility, 0.0f);
  sf = 100.0f;  // Set target to 100
  sf.force();   // Force current to target
  EXPECT_FLOAT_EQ(100.0f, sf.current());
  EXPECT_FLOAT_EQ(100.0f, sf.target());
}

// update() tests
TEST_F(SmoothFloatTest, UpdateMovesTowardTarget) {
  SmoothFloat sf(kAgility, 0.0f);
  sf = 100.0f;  // Set target to 100
  sf.update(0.1);
  EXPECT_GT(sf.current(), 0.0f);
  EXPECT_LT(sf.current(), 100.0f);
}

TEST_F(SmoothFloatTest, UpdateNoChangeWhenAtTarget) {
  SmoothFloat sf(kAgility, 50.0f);
  f32 before = sf.current();
  sf.update(0.1);
  EXPECT_FLOAT_EQ(before, sf.current());
}

TEST_F(SmoothFloatTest, UpdateMovesProportionalToAgility) {
  SmoothFloat slow(1.0f, 0.0f);
  SmoothFloat fast(10.0f, 0.0f);
  slow = 100.0f;
  fast = 100.0f;
  slow.update(0.1);
  fast.update(0.1);
  EXPECT_LT(slow.current(), fast.current());
}

TEST_F(SmoothFloatTest, UpdateMovesProportionalToDeltaTime) {
  SmoothFloat sf1(kAgility, 0.0f);
  SmoothFloat sf2(kAgility, 0.0f);
  sf1 = 100.0f;
  sf2 = 100.0f;
  sf1.update(0.05);
  sf2.update(0.1);
  EXPECT_LT(sf1.current(), sf2.current());
}

// Assignment operator tests
TEST_F(SmoothFloatTest, AssignmentSetsTarget) {
  SmoothFloat sf(kAgility, 0.0f);
  sf = 42.0f;
  EXPECT_FLOAT_EQ(42.0f, sf.target());
  EXPECT_FLOAT_EQ(0.0f, sf.current());  // Current unchanged
}

TEST_F(SmoothFloatTest, AssignmentReturnsSelf) {
  SmoothFloat sf(kAgility, 0.0f);
  SmoothFloat& result = (sf = 42.0f);
  EXPECT_EQ(&sf, &result);
}

// Compound assignment tests
TEST_F(SmoothFloatTest, PlusEqualsAddsToTarget) {
  SmoothFloat sf(kAgility, 10.0f);
  sf += 5.0f;
  EXPECT_FLOAT_EQ(15.0f, sf.target());
}

TEST_F(SmoothFloatTest, MinusEqualsSubtractsFromTarget) {
  SmoothFloat sf(kAgility, 10.0f);
  sf -= 3.0f;
  EXPECT_FLOAT_EQ(7.0f, sf.target());
}

// Comparison operator tests
TEST_F(SmoothFloatTest, EqualityComparesCurrent) {
  SmoothFloat sf1(kAgility, 10.0f);
  SmoothFloat sf2(kAgility, 10.0f);
  EXPECT_TRUE(sf1 == sf2);
}

TEST_F(SmoothFloatTest, EqualityWithDifferentCurrent) {
  SmoothFloat sf1(kAgility, 10.0f);
  SmoothFloat sf2(kAgility, 20.0f);
  EXPECT_FALSE(sf1 == sf2);
}

TEST_F(SmoothFloatTest, InequalityComparesCurrent) {
  SmoothFloat sf1(kAgility, 10.0f);
  SmoothFloat sf2(kAgility, 20.0f);
  EXPECT_TRUE(sf1 != sf2);
}

// Note: The comparison operators in SmoothFloat compare fabs(current_ -
// other.current_) against epsilon_f, not the actual current values. This is
// unusual behavior.
TEST_F(SmoothFloatTest, GreaterThanComparesAbsDifference) {
  SmoothFloat sf1(kAgility, 20.0f);
  SmoothFloat sf2(kAgility, 10.0f);
  // |20 - 10| = 10 > epsilon_f, so this should be true
  EXPECT_TRUE(sf1 > sf2);
}

TEST_F(SmoothFloatTest, LessThanComparesAbsDifference) {
  SmoothFloat sf1(kAgility, 10.0f);
  SmoothFloat sf2(kAgility, 10.0f);
  // |10 - 10| = 0 < epsilon_f, so this should be true
  EXPECT_TRUE(sf1 < sf2);
}

TEST_F(SmoothFloatTest, GreaterOrEqualComparesAbsDifference) {
  SmoothFloat sf1(kAgility, 20.0f);
  SmoothFloat sf2(kAgility, 10.0f);
  // |20 - 10| = 10 >= epsilon_f
  EXPECT_TRUE(sf1 >= sf2);
}

TEST_F(SmoothFloatTest, LessOrEqualComparesAbsDifference) {
  SmoothFloat sf1(kAgility, 10.0f);
  SmoothFloat sf2(kAgility, 10.0f);
  // |10 - 10| = 0 <= epsilon_f
  EXPECT_TRUE(sf1 <= sf2);
}

// Conversion operator tests
TEST_F(SmoothFloatTest, ConversionReturnsCurrent) {
  SmoothFloat sf(kAgility, 42.0f);
  f32 value = sf;
  EXPECT_FLOAT_EQ(42.0f, value);
}

TEST_F(SmoothFloatTest, ConversionUsableInExpressions) {
  SmoothFloat sf(kAgility, 10.0f);
  f32 result = sf + 5.0f;
  EXPECT_FLOAT_EQ(15.0f, result);
}

// Getter/setter tests
TEST_F(SmoothFloatTest, AgilityGetterReturnsAgility) {
  SmoothFloat sf(kAgility);
  EXPECT_FLOAT_EQ(kAgility, sf.agility());
}

TEST_F(SmoothFloatTest, AgilitySetterChangesAgility) {
  SmoothFloat sf(kAgility);
  sf.agility(10.0f);
  EXPECT_FLOAT_EQ(10.0f, sf.agility());
}

TEST_F(SmoothFloatTest, CurrentGetterReturnsCurrent) {
  SmoothFloat sf(kAgility, kStartValue);
  EXPECT_FLOAT_EQ(kStartValue, sf.current());
}

TEST_F(SmoothFloatTest, CurrentSetterChangesCurrent) {
  SmoothFloat sf(kAgility, kStartValue);
  sf.current(99.0f);
  EXPECT_FLOAT_EQ(99.0f, sf.current());
}

TEST_F(SmoothFloatTest, TargetGetterReturnsTarget) {
  SmoothFloat sf(kAgility, kStartValue);
  EXPECT_FLOAT_EQ(kStartValue, sf.target());
}

TEST_F(SmoothFloatTest, TargetSetterChangesTarget) {
  SmoothFloat sf(kAgility, kStartValue);
  sf.target(200.0f);
  EXPECT_FLOAT_EQ(200.0f, sf.target());
}

// Integration test
TEST_F(SmoothFloatTest, ConvergesToTargetOverTime) {
  SmoothFloat sf(10.0f, 0.0f);
  sf = 100.0f;

  // Simulate many updates
  for (int i = 0; i < 100; ++i) {
    sf.update(0.1);
  }

  // Should be very close to target after many iterations
  EXPECT_NEAR(100.0f, sf.current(), 0.1f);
}

}  // namespace
}  // namespace uinta
