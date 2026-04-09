#include "uinta/math/direction.h"

#include <gtest/gtest.h>

#include <glm/geometric.hpp>

namespace uinta {
namespace {

// WorldForward tests
TEST(DirectionTest, WorldForwardIsNegativeZ) {
  EXPECT_EQ(glm::vec3(0, 0, -1), WorldForward);
}

TEST(DirectionTest, WorldForwardXIsZero) { EXPECT_EQ(0.0f, WorldForward.x); }

TEST(DirectionTest, WorldForwardYIsZero) { EXPECT_EQ(0.0f, WorldForward.y); }

TEST(DirectionTest, WorldForwardZIsNegativeOne) {
  EXPECT_EQ(-1.0f, WorldForward.z);
}

TEST(DirectionTest, WorldForwardIsUnitVector) {
  EXPECT_FLOAT_EQ(1.0f, glm::length(WorldForward));
}

// WorldRight tests
TEST(DirectionTest, WorldRightIsPositiveX) {
  EXPECT_EQ(glm::vec3(1, 0, 0), WorldRight);
}

TEST(DirectionTest, WorldRightXIsOne) { EXPECT_EQ(1.0f, WorldRight.x); }

TEST(DirectionTest, WorldRightYIsZero) { EXPECT_EQ(0.0f, WorldRight.y); }

TEST(DirectionTest, WorldRightZIsZero) { EXPECT_EQ(0.0f, WorldRight.z); }

TEST(DirectionTest, WorldRightIsUnitVector) {
  EXPECT_FLOAT_EQ(1.0f, glm::length(WorldRight));
}

// WorldUp tests
TEST(DirectionTest, WorldUpIsPositiveY) {
  EXPECT_EQ(glm::vec3(0, 1, 0), WorldUp);
}

TEST(DirectionTest, WorldUpXIsZero) { EXPECT_EQ(0.0f, WorldUp.x); }

TEST(DirectionTest, WorldUpYIsOne) { EXPECT_EQ(1.0f, WorldUp.y); }

TEST(DirectionTest, WorldUpZIsZero) { EXPECT_EQ(0.0f, WorldUp.z); }

TEST(DirectionTest, WorldUpIsUnitVector) {
  EXPECT_FLOAT_EQ(1.0f, glm::length(WorldUp));
}

// WorldHorizontal tests
TEST(DirectionTest, WorldHorizontalIsSumOfRightAndForward) {
  EXPECT_EQ(WorldRight + WorldForward, WorldHorizontal);
}

TEST(DirectionTest, WorldHorizontalXIsOne) {
  EXPECT_EQ(1.0f, WorldHorizontal.x);
}

TEST(DirectionTest, WorldHorizontalYIsZero) {
  EXPECT_EQ(0.0f, WorldHorizontal.y);
}

TEST(DirectionTest, WorldHorizontalZIsNegativeOne) {
  EXPECT_EQ(-1.0f, WorldHorizontal.z);
}

// Orthogonality tests
TEST(DirectionTest, ForwardAndRightAreOrthogonal) {
  EXPECT_FLOAT_EQ(0.0f, glm::dot(WorldForward, WorldRight));
}

TEST(DirectionTest, ForwardAndUpAreOrthogonal) {
  EXPECT_FLOAT_EQ(0.0f, glm::dot(WorldForward, WorldUp));
}

TEST(DirectionTest, RightAndUpAreOrthogonal) {
  EXPECT_FLOAT_EQ(0.0f, glm::dot(WorldRight, WorldUp));
}

// Cross product relationship tests
TEST(DirectionTest, CrossForwardRightGivesNegativeUp) {
  // In a right-handed coordinate system: forward x right = -up
  auto result = glm::cross(WorldForward, WorldRight);
  EXPECT_FLOAT_EQ(-WorldUp.x, result.x);
  EXPECT_FLOAT_EQ(-WorldUp.y, result.y);
  EXPECT_FLOAT_EQ(-WorldUp.z, result.z);
}

TEST(DirectionTest, CrossRightUpGivesNegativeForward) {
  // In OpenGL's right-handed coordinate system with -Z forward
  auto result = glm::cross(WorldRight, WorldUp);
  EXPECT_FLOAT_EQ(-WorldForward.x, result.x);
  EXPECT_FLOAT_EQ(-WorldForward.y, result.y);
  EXPECT_FLOAT_EQ(-WorldForward.z, result.z);
}

TEST(DirectionTest, CrossUpForwardGivesNegativeRight) {
  // In OpenGL's right-handed coordinate system with -Z forward
  auto result = glm::cross(WorldUp, WorldForward);
  EXPECT_FLOAT_EQ(-WorldRight.x, result.x);
  EXPECT_FLOAT_EQ(-WorldRight.y, result.y);
  EXPECT_FLOAT_EQ(-WorldRight.z, result.z);
}

}  // namespace
}  // namespace uinta
