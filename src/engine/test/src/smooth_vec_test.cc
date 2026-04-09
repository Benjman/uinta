#include "uinta/math/smooth_vec.h"

#include <gtest/gtest.h>

namespace uinta {
namespace {

// ============================================================================
// SmoothVec2 Tests
// ============================================================================

class SmoothVec2Test : public ::testing::Test {
 protected:
  static constexpr f32 kAgility = 5.0f;
};

// Constructor tests
TEST_F(SmoothVec2Test, ConstructorSetsComponents) {
  SmoothVec2 sv(kAgility, 1.0f, 2.0f);
  EXPECT_FLOAT_EQ(1.0f, sv.x().current());
  EXPECT_FLOAT_EQ(2.0f, sv.y().current());
}

TEST_F(SmoothVec2Test, ConstructorWithDefaultsZero) {
  SmoothVec2 sv(kAgility);
  EXPECT_FLOAT_EQ(0.0f, sv.x().current());
  EXPECT_FLOAT_EQ(0.0f, sv.y().current());
}

TEST_F(SmoothVec2Test, ConstructorFromGlmVec2) {
  glm::vec2 v(3.0f, 4.0f);
  SmoothVec2 sv(kAgility, v);
  EXPECT_FLOAT_EQ(3.0f, sv.x().current());
  EXPECT_FLOAT_EQ(4.0f, sv.y().current());
}

// agility() tests
TEST_F(SmoothVec2Test, AgilitySetsAllComponents) {
  SmoothVec2 sv(1.0f);
  sv.agility(10.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().agility());
  EXPECT_FLOAT_EQ(10.0f, sv.y().agility());
}

// force() tests
TEST_F(SmoothVec2Test, ForceSetsBothCurrentAndTarget) {
  SmoothVec2 sv(kAgility, 0.0f, 0.0f);
  sv = glm::vec2(100.0f, 200.0f);
  sv.force(glm::vec2(50.0f, 60.0f));
  EXPECT_FLOAT_EQ(50.0f, sv.x().current());
  EXPECT_FLOAT_EQ(60.0f, sv.y().current());
  EXPECT_FLOAT_EQ(50.0f, sv.x().target());
  EXPECT_FLOAT_EQ(60.0f, sv.y().target());
}

TEST_F(SmoothVec2Test, ForceNoArgUsesTarget) {
  SmoothVec2 sv(kAgility, 0.0f, 0.0f);
  sv = glm::vec2(100.0f, 200.0f);
  sv.force();
  EXPECT_FLOAT_EQ(100.0f, sv.x().current());
  EXPECT_FLOAT_EQ(200.0f, sv.y().current());
}

// update() tests
TEST_F(SmoothVec2Test, UpdateMovesTowardTarget) {
  SmoothVec2 sv(kAgility, 0.0f, 0.0f);
  sv = glm::vec2(100.0f, 100.0f);
  sv.update(0.1);
  EXPECT_GT(sv.x().current(), 0.0f);
  EXPECT_GT(sv.y().current(), 0.0f);
  EXPECT_LT(sv.x().current(), 100.0f);
  EXPECT_LT(sv.y().current(), 100.0f);
}

// Operator tests
TEST_F(SmoothVec2Test, AssignmentSetsTarget) {
  SmoothVec2 sv(kAgility);
  sv = glm::vec2(10.0f, 20.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().target());
  EXPECT_FLOAT_EQ(20.0f, sv.y().target());
}

TEST_F(SmoothVec2Test, PlusEqualsAddsToTarget) {
  SmoothVec2 sv(kAgility, 10.0f, 20.0f);
  sv += glm::vec2(5.0f, 10.0f);
  EXPECT_FLOAT_EQ(15.0f, sv.x().target());
  EXPECT_FLOAT_EQ(30.0f, sv.y().target());
}

TEST_F(SmoothVec2Test, MinusEqualsSubtractsFromTarget) {
  SmoothVec2 sv(kAgility, 10.0f, 20.0f);
  sv -= glm::vec2(3.0f, 5.0f);
  EXPECT_FLOAT_EQ(7.0f, sv.x().target());
  EXPECT_FLOAT_EQ(15.0f, sv.y().target());
}

// Comparison operator tests
TEST_F(SmoothVec2Test, EqualityComparesComponents) {
  SmoothVec2 sv1(kAgility, 10.0f, 20.0f);
  SmoothVec2 sv2(kAgility, 10.0f, 20.0f);
  EXPECT_TRUE(sv1 == sv2);
}

TEST_F(SmoothVec2Test, InequalityComparesComponents) {
  SmoothVec2 sv1(kAgility, 10.0f, 20.0f);
  SmoothVec2 sv2(kAgility, 15.0f, 25.0f);
  EXPECT_TRUE(sv1 != sv2);
}

// Conversion operator tests
TEST_F(SmoothVec2Test, ConversionToGlmVec2) {
  SmoothVec2 sv(kAgility, 10.0f, 20.0f);
  glm::vec2 v = sv;
  EXPECT_FLOAT_EQ(10.0f, v.x);
  EXPECT_FLOAT_EQ(20.0f, v.y);
}

// Component accessor tests
TEST_F(SmoothVec2Test, XGetterReturnsX) {
  SmoothVec2 sv(kAgility, 10.0f, 20.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().current());
}

TEST_F(SmoothVec2Test, YGetterReturnsY) {
  SmoothVec2 sv(kAgility, 10.0f, 20.0f);
  EXPECT_FLOAT_EQ(20.0f, sv.y().current());
}

// ============================================================================
// SmoothVec3 Tests
// ============================================================================

class SmoothVec3Test : public ::testing::Test {
 protected:
  static constexpr f32 kAgility = 5.0f;
};

// Constructor tests
TEST_F(SmoothVec3Test, ConstructorSetsComponents) {
  SmoothVec3 sv(kAgility, 1.0f, 2.0f, 3.0f);
  EXPECT_FLOAT_EQ(1.0f, sv.x().current());
  EXPECT_FLOAT_EQ(2.0f, sv.y().current());
  EXPECT_FLOAT_EQ(3.0f, sv.z().current());
}

TEST_F(SmoothVec3Test, ConstructorWithDefaultsZero) {
  SmoothVec3 sv(kAgility);
  EXPECT_FLOAT_EQ(0.0f, sv.x().current());
  EXPECT_FLOAT_EQ(0.0f, sv.y().current());
  EXPECT_FLOAT_EQ(0.0f, sv.z().current());
}

TEST_F(SmoothVec3Test, ConstructorFromGlmVec3) {
  glm::vec3 v(3.0f, 4.0f, 5.0f);
  SmoothVec3 sv(kAgility, v);
  EXPECT_FLOAT_EQ(3.0f, sv.x().current());
  EXPECT_FLOAT_EQ(4.0f, sv.y().current());
  EXPECT_FLOAT_EQ(5.0f, sv.z().current());
}

// agility() tests
TEST_F(SmoothVec3Test, AgilitySetsAllComponents) {
  SmoothVec3 sv(1.0f);
  sv.agility(10.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().agility());
  EXPECT_FLOAT_EQ(10.0f, sv.y().agility());
  EXPECT_FLOAT_EQ(10.0f, sv.z().agility());
}

// force() tests
TEST_F(SmoothVec3Test, ForceSetsBothCurrentAndTarget) {
  SmoothVec3 sv(kAgility, 0.0f, 0.0f, 0.0f);
  sv = glm::vec3(100.0f, 200.0f, 300.0f);
  sv.force(glm::vec3(50.0f, 60.0f, 70.0f));
  EXPECT_FLOAT_EQ(50.0f, sv.x().current());
  EXPECT_FLOAT_EQ(60.0f, sv.y().current());
  EXPECT_FLOAT_EQ(70.0f, sv.z().current());
  EXPECT_FLOAT_EQ(50.0f, sv.x().target());
  EXPECT_FLOAT_EQ(60.0f, sv.y().target());
  EXPECT_FLOAT_EQ(70.0f, sv.z().target());
}

TEST_F(SmoothVec3Test, ForceNoArgUsesTarget) {
  SmoothVec3 sv(kAgility, 0.0f, 0.0f, 0.0f);
  sv = glm::vec3(100.0f, 200.0f, 300.0f);
  sv.force();
  EXPECT_FLOAT_EQ(100.0f, sv.x().current());
  EXPECT_FLOAT_EQ(200.0f, sv.y().current());
  EXPECT_FLOAT_EQ(300.0f, sv.z().current());
}

// update() tests
TEST_F(SmoothVec3Test, UpdateMovesTowardTarget) {
  SmoothVec3 sv(kAgility, 0.0f, 0.0f, 0.0f);
  sv = glm::vec3(100.0f, 100.0f, 100.0f);
  sv.update(0.1);
  EXPECT_GT(sv.x().current(), 0.0f);
  EXPECT_GT(sv.y().current(), 0.0f);
  EXPECT_GT(sv.z().current(), 0.0f);
  EXPECT_LT(sv.x().current(), 100.0f);
  EXPECT_LT(sv.y().current(), 100.0f);
  EXPECT_LT(sv.z().current(), 100.0f);
}

// Operator tests
TEST_F(SmoothVec3Test, AssignmentSetsTarget) {
  SmoothVec3 sv(kAgility);
  sv = glm::vec3(10.0f, 20.0f, 30.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().target());
  EXPECT_FLOAT_EQ(20.0f, sv.y().target());
  EXPECT_FLOAT_EQ(30.0f, sv.z().target());
}

TEST_F(SmoothVec3Test, PlusEqualsAddsToTarget) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  sv += glm::vec3(5.0f, 10.0f, 15.0f);
  EXPECT_FLOAT_EQ(15.0f, sv.x().target());
  EXPECT_FLOAT_EQ(30.0f, sv.y().target());
  EXPECT_FLOAT_EQ(45.0f, sv.z().target());
}

TEST_F(SmoothVec3Test, MinusEqualsSubtractsFromTarget) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  sv -= glm::vec3(3.0f, 5.0f, 7.0f);
  EXPECT_FLOAT_EQ(7.0f, sv.x().target());
  EXPECT_FLOAT_EQ(15.0f, sv.y().target());
  EXPECT_FLOAT_EQ(23.0f, sv.z().target());
}

// Comparison operator tests
TEST_F(SmoothVec3Test, EqualityComparesComponents) {
  SmoothVec3 sv1(kAgility, 10.0f, 20.0f, 30.0f);
  SmoothVec3 sv2(kAgility, 10.0f, 20.0f, 30.0f);
  EXPECT_TRUE(sv1 == sv2);
}

TEST_F(SmoothVec3Test, InequalityComparesComponents) {
  SmoothVec3 sv1(kAgility, 10.0f, 20.0f, 30.0f);
  SmoothVec3 sv2(kAgility, 15.0f, 25.0f, 35.0f);
  EXPECT_TRUE(sv1 != sv2);
}

// Conversion operator tests
TEST_F(SmoothVec3Test, ConversionToGlmVec3) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  glm::vec3 v = sv;
  EXPECT_FLOAT_EQ(10.0f, v.x);
  EXPECT_FLOAT_EQ(20.0f, v.y);
  EXPECT_FLOAT_EQ(30.0f, v.z);
}

// Component accessor tests
TEST_F(SmoothVec3Test, XGetterReturnsX) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  EXPECT_FLOAT_EQ(10.0f, sv.x().current());
}

TEST_F(SmoothVec3Test, YGetterReturnsY) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  EXPECT_FLOAT_EQ(20.0f, sv.y().current());
}

TEST_F(SmoothVec3Test, ZGetterReturnsZ) {
  SmoothVec3 sv(kAgility, 10.0f, 20.0f, 30.0f);
  EXPECT_FLOAT_EQ(30.0f, sv.z().current());
}

// Integration test
TEST_F(SmoothVec3Test, ConvergesToTargetOverTime) {
  SmoothVec3 sv(10.0f, 0.0f, 0.0f, 0.0f);
  sv = glm::vec3(100.0f, 200.0f, 300.0f);

  for (int i = 0; i < 100; ++i) {
    sv.update(0.1);
  }

  EXPECT_NEAR(100.0f, sv.x().current(), 0.1f);
  EXPECT_NEAR(200.0f, sv.y().current(), 0.1f);
  EXPECT_NEAR(300.0f, sv.z().current(), 0.1f);
}

}  // namespace
}  // namespace uinta
