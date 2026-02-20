#include "uinta/math/aabb.h"

#include <gtest/gtest.h>

#include "uinta/types.h"

namespace uinta {

TEST(AABB, DefaultConstructor) {
  AABB aabb;
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb.min);
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, Empty) {
  AABB aabb = AABB::empty();
  EXPECT_FALSE(aabb.isValid());
  EXPECT_GT(aabb.min.x, aabb.max.x);
  EXPECT_GT(aabb.min.y, aabb.max.y);
  EXPECT_GT(aabb.min.z, aabb.max.z);
}

TEST(AABB, ConstructFromVec3Points) {
  std::vector<glm::vec3> points = {{1, 2, 3}, {4, 5, 6}, {-1, -2, -3}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(-1, -2, -3), aabb.min);
  EXPECT_EQ(glm::vec3(4, 5, 6), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, ConstructFromVec2Points) {
  std::vector<glm::vec2> points = {{1, 2}, {4, 5}, {-1, -2}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(-1, -2, 0), aabb.min);
  EXPECT_EQ(glm::vec3(4, 5, 0), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, ConstructFromRawFloats2D) {
  std::vector<f32> data = {1.0f, 2.0f, 4.0f, 5.0f, -1.0f, -2.0f};
  AABB aabb(data, 2);
  EXPECT_EQ(glm::vec3(-1, -2, 0), aabb.min);
  EXPECT_EQ(glm::vec3(4, 5, 0), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, ConstructFromRawFloats3D) {
  std::vector<f32> data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, -1.0f, -2.0f,
                            -3.0f};
  AABB aabb(data, 3);
  EXPECT_EQ(glm::vec3(-1, -2, -3), aabb.min);
  EXPECT_EQ(glm::vec3(4, 5, 6), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, SinglePoint) {
  std::vector<glm::vec3> points = {{1, 2, 3}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(1, 2, 3), aabb.min);
  EXPECT_EQ(glm::vec3(1, 2, 3), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, Center) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 20, 30}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(5, 10, 15), aabb.center());
}

TEST(AABB, Size) {
  std::vector<glm::vec3> points = {{1, 2, 3}, {4, 7, 13}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(3, 5, 10), aabb.size());
}

TEST(AABB, Volume) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {2, 3, 4}};
  AABB aabb(points);
  EXPECT_FLOAT_EQ(24.0f, aabb.volume());
}

TEST(AABB, VolumeZero) {
  std::vector<glm::vec3> points = {{1, 1, 1}};
  AABB aabb(points);
  EXPECT_FLOAT_EQ(0.0f, aabb.volume());
}

TEST(AABB, SurfaceArea) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {2, 3, 4}};
  AABB aabb(points);
  // 2*(2*3 + 3*4 + 4*2) = 2*(6 + 12 + 8) = 52
  EXPECT_FLOAT_EQ(52.0f, aabb.surfaceArea());
}

TEST(AABB, SurfaceAreaZero) {
  std::vector<glm::vec3> points = {{1, 1, 1}};
  AABB aabb(points);
  EXPECT_FLOAT_EQ(0.0f, aabb.surfaceArea());
}

TEST(AABB, ContainsPointInside) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  EXPECT_TRUE(aabb.contains(glm::vec3(5, 5, 5)));
}

TEST(AABB, ContainsPointOnBoundary) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  EXPECT_TRUE(aabb.contains(glm::vec3(0, 0, 0)));
  EXPECT_TRUE(aabb.contains(glm::vec3(10, 10, 10)));
  EXPECT_TRUE(aabb.contains(glm::vec3(5, 0, 10)));
}

TEST(AABB, ContainsPointOutside) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  EXPECT_FALSE(aabb.contains(glm::vec3(-1, 5, 5)));
  EXPECT_FALSE(aabb.contains(glm::vec3(5, 11, 5)));
  EXPECT_FALSE(aabb.contains(glm::vec3(5, 5, -0.1f)));
}

TEST(AABB, IntersectsOverlapping) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{5, 5, 5}, {15, 15, 15}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  EXPECT_TRUE(aabb1.intersects(aabb2));
  EXPECT_TRUE(aabb2.intersects(aabb1));
}

TEST(AABB, IntersectsContained) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{2, 2, 2}, {8, 8, 8}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  EXPECT_TRUE(aabb1.intersects(aabb2));
  EXPECT_TRUE(aabb2.intersects(aabb1));
}

TEST(AABB, IntersectsTouching) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{10, 10, 10}, {20, 20, 20}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  EXPECT_TRUE(aabb1.intersects(aabb2));
  EXPECT_TRUE(aabb2.intersects(aabb1));
}

TEST(AABB, IntersectsNonOverlapping) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{11, 11, 11}, {20, 20, 20}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  EXPECT_FALSE(aabb1.intersects(aabb2));
  EXPECT_FALSE(aabb2.intersects(aabb1));
}

TEST(AABB, IntersectsPartialOverlap) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{5, 11, 5}, {15, 20, 15}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  EXPECT_FALSE(aabb1.intersects(aabb2));
  EXPECT_FALSE(aabb2.intersects(aabb1));
}

TEST(AABB, ExpandByPoint) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  aabb.expand(glm::vec3(15, 5, 5));
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb.min);
  EXPECT_EQ(glm::vec3(15, 10, 10), aabb.max);
}

TEST(AABB, ExpandByPointInside) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  aabb.expand(glm::vec3(5, 5, 5));
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb.min);
  EXPECT_EQ(glm::vec3(10, 10, 10), aabb.max);
}

TEST(AABB, ExpandByPointNegative) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  aabb.expand(glm::vec3(-5, -5, -5));
  EXPECT_EQ(glm::vec3(-5, -5, -5), aabb.min);
  EXPECT_EQ(glm::vec3(10, 10, 10), aabb.max);
}

TEST(AABB, ExpandByAABB) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{5, 5, 5}, {15, 15, 15}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  aabb1.expand(aabb2);
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb1.min);
  EXPECT_EQ(glm::vec3(15, 15, 15), aabb1.max);
}

TEST(AABB, ExpandByAABBContained) {
  std::vector<glm::vec3> points1 = {{0, 0, 0}, {10, 10, 10}};
  std::vector<glm::vec3> points2 = {{2, 2, 2}, {8, 8, 8}};
  AABB aabb1(points1);
  AABB aabb2(points2);
  aabb1.expand(aabb2);
  EXPECT_EQ(glm::vec3(0, 0, 0), aabb1.min);
  EXPECT_EQ(glm::vec3(10, 10, 10), aabb1.max);
}

TEST(AABB, ExpandEmptyByPoint) {
  AABB aabb = AABB::empty();
  aabb.expand(glm::vec3(5, 5, 5));
  EXPECT_TRUE(aabb.isValid());
  EXPECT_EQ(glm::vec3(5, 5, 5), aabb.min);
  EXPECT_EQ(glm::vec3(5, 5, 5), aabb.max);
}

TEST(AABB, ExpandEmptyByMultiplePoints) {
  AABB aabb = AABB::empty();
  aabb.expand(glm::vec3(1, 2, 3));
  aabb.expand(glm::vec3(4, 5, 6));
  aabb.expand(glm::vec3(-1, -2, -3));
  EXPECT_TRUE(aabb.isValid());
  EXPECT_EQ(glm::vec3(-1, -2, -3), aabb.min);
  EXPECT_EQ(glm::vec3(4, 5, 6), aabb.max);
}

TEST(AABB, IsValidTrue) {
  std::vector<glm::vec3> points = {{0, 0, 0}, {10, 10, 10}};
  AABB aabb(points);
  EXPECT_TRUE(aabb.isValid());
}

TEST(AABB, IsValidFalse) {
  AABB aabb;
  aabb.min = glm::vec3(10, 10, 10);
  aabb.max = glm::vec3(0, 0, 0);
  EXPECT_FALSE(aabb.isValid());
}

TEST(AABB, NegativeCoordinates) {
  std::vector<glm::vec3> points = {{-10, -20, -30}, {-5, -10, -15}};
  AABB aabb(points);
  EXPECT_EQ(glm::vec3(-10, -20, -30), aabb.min);
  EXPECT_EQ(glm::vec3(-5, -10, -15), aabb.max);
  EXPECT_TRUE(aabb.isValid());
}

}  // namespace uinta
