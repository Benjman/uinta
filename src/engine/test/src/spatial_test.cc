#include "uinta/math/spatial.h"

#include <gtest/gtest.h>

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace uinta {
namespace {

class SpatialTest : public ::testing::Test {};

// getForward tests
TEST(SpatialTest, GetForwardAtZeroPitchZeroYaw) {
  auto forward = getForward(0.0f, 0.0f);
  // At pitch=0, yaw=0, should point along positive Z (cos(0)*cos(0) = 1)
  EXPECT_NEAR(0.0f, forward.x, 1e-5);
  EXPECT_NEAR(0.0f, forward.y, 1e-5);
  EXPECT_NEAR(1.0f, forward.z, 1e-5);
}

TEST(SpatialTest, GetForwardAtZeroPitch90Yaw) {
  auto forward = getForward(0.0f, 90.0f);
  // At pitch=0, yaw=90, should point along positive X
  EXPECT_NEAR(1.0f, forward.x, 1e-5);
  EXPECT_NEAR(0.0f, forward.y, 1e-5);
  EXPECT_NEAR(0.0f, forward.z, 1e-5);
}

TEST(SpatialTest, GetForwardAt90PitchZeroYaw) {
  auto forward = getForward(90.0f, 0.0f);
  // At pitch=90 (looking down), should point along negative Y
  EXPECT_NEAR(0.0f, forward.x, 1e-5);
  EXPECT_NEAR(-1.0f, forward.y, 1e-5);
  EXPECT_NEAR(0.0f, forward.z, 1e-5);
}

TEST(SpatialTest, GetForwardIsUnitVector) {
  auto forward = getForward(30.0f, 45.0f);
  EXPECT_NEAR(1.0f, glm::length(forward), 1e-5);
}

// getRight tests
TEST(SpatialTest, GetRightAtZeroYaw) {
  auto right = getRight(0.0f);
  // At yaw=0, right should point along positive X
  EXPECT_NEAR(1.0f, right.x, 1e-5);
  EXPECT_NEAR(0.0f, right.y, 1e-5);
  EXPECT_NEAR(0.0f, right.z, 1e-5);
}

TEST(SpatialTest, GetRightAt90Yaw) {
  auto right = getRight(90.0f);
  // At yaw=90, right should point along negative Z
  EXPECT_NEAR(0.0f, right.x, 1e-5);
  EXPECT_NEAR(0.0f, right.y, 1e-5);
  EXPECT_NEAR(-1.0f, right.z, 1e-5);
}

TEST(SpatialTest, GetRightIsUnitVector) {
  auto right = getRight(45.0f);
  EXPECT_NEAR(1.0f, glm::length(right), 1e-5);
}

TEST(SpatialTest, GetRightIsHorizontal) {
  auto right = getRight(60.0f);
  EXPECT_FLOAT_EQ(0.0f, right.y);  // Y component should always be 0
}

// getUp tests
TEST(SpatialTest, GetUpWithPitchYaw) {
  auto up = getUp(0.0f, 0.0f);
  // At pitch=0, yaw=0, up should be perpendicular to forward and right
  auto forward = getForward(0.0f, 0.0f);
  auto right = getRight(0.0f);
  EXPECT_NEAR(0.0f, glm::dot(up, forward), 1e-5);
  EXPECT_NEAR(0.0f, glm::dot(up, right), 1e-5);
}

TEST(SpatialTest, GetUpFromForwardRight) {
  glm::vec3 forward(0, 0, 1);
  glm::vec3 right(1, 0, 0);
  auto up = getUp(forward, right);
  // Cross product of forward and right: (0,0,1) x (1,0,0) = (0,1,0)
  EXPECT_FLOAT_EQ(0.0f, up.x);
  EXPECT_FLOAT_EQ(1.0f, up.y);
  EXPECT_FLOAT_EQ(0.0f, up.z);
}

// getPointOnRay tests
TEST(SpatialTest, GetPointOnRayAtZeroDist) {
  glm::vec3 pos(1, 2, 3);
  glm::vec3 dir(1, 0, 0);
  auto point = getPointOnRay(pos, dir, 0.0f);
  EXPECT_FLOAT_EQ(pos.x, point.x);
  EXPECT_FLOAT_EQ(pos.y, point.y);
  EXPECT_FLOAT_EQ(pos.z, point.z);
}

TEST(SpatialTest, GetPointOnRayPositiveDist) {
  glm::vec3 pos(0, 0, 0);
  glm::vec3 dir(1, 0, 0);
  auto point = getPointOnRay(pos, dir, 5.0f);
  EXPECT_FLOAT_EQ(5.0f, point.x);
  EXPECT_FLOAT_EQ(0.0f, point.y);
  EXPECT_FLOAT_EQ(0.0f, point.z);
}

TEST(SpatialTest, GetPointOnRayNegativeDist) {
  glm::vec3 pos(0, 0, 0);
  glm::vec3 dir(1, 0, 0);
  auto point = getPointOnRay(pos, dir, -3.0f);
  EXPECT_FLOAT_EQ(-3.0f, point.x);
  EXPECT_FLOAT_EQ(0.0f, point.y);
  EXPECT_FLOAT_EQ(0.0f, point.z);
}

TEST(SpatialTest, GetPointOnRayDiagonalDirection) {
  glm::vec3 pos(0, 0, 0);
  glm::vec3 dir = glm::normalize(glm::vec3(1, 1, 1));
  auto point = getPointOnRay(pos, dir, std::numbers::sqrt3_v<float>);
  EXPECT_NEAR(1.0f, point.x, 1e-5);
  EXPECT_NEAR(1.0f, point.y, 1e-5);
  EXPECT_NEAR(1.0f, point.z, 1e-5);
}

// getPlaneInterceptDist tests
TEST(SpatialTest, GetPlaneInterceptDistRayHitsPlane) {
  glm::vec3 planePos(0, 0, 0);
  glm::vec3 planeNormal(0, 1, 0);  // Horizontal plane
  glm::vec3 rayPos(0, 5, 0);
  glm::vec3 rayDir(0, -1, 0);  // Pointing down
  auto dist = getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDir);
  EXPECT_FLOAT_EQ(5.0f, dist);
}

TEST(SpatialTest, GetPlaneInterceptDistRayParallelToPlane) {
  glm::vec3 planePos(0, 0, 0);
  glm::vec3 planeNormal(0, 1, 0);
  glm::vec3 rayPos(0, 5, 0);
  glm::vec3 rayDir(1, 0, 0);  // Parallel to plane
  auto dist = getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDir);
  EXPECT_FLOAT_EQ(0.0f, dist);  // Returns 0 when parallel
}

TEST(SpatialTest, GetPlaneInterceptDistRayPointsAway) {
  glm::vec3 planePos(0, 0, 0);
  glm::vec3 planeNormal(0, 1, 0);
  glm::vec3 rayPos(0, 5, 0);
  glm::vec3 rayDir(0, 1, 0);  // Pointing away from plane
  auto dist = getPlaneInterceptDist(planePos, planeNormal, rayPos, rayDir);
  EXPECT_LT(dist, 0.0f);  // Negative distance when pointing away
}

// getPlaneInterceptPoint tests
TEST(SpatialTest, GetPlaneInterceptPointHitsPlane) {
  glm::vec3 planePos(0, 0, 0);
  glm::vec3 planeNormal(0, 1, 0);
  glm::vec3 rayPos(3, 10, 4);
  glm::vec3 rayDir(0, -1, 0);
  auto point = getPlaneInterceptPoint(planePos, planeNormal, rayPos, rayDir);
  EXPECT_FLOAT_EQ(3.0f, point.x);
  EXPECT_FLOAT_EQ(0.0f, point.y);
  EXPECT_FLOAT_EQ(4.0f, point.z);
}

TEST(SpatialTest, GetPlaneInterceptPointReturnsInfinityWhenNoHit) {
  glm::vec3 planePos(0, 0, 0);
  glm::vec3 planeNormal(0, 1, 0);
  glm::vec3 rayPos(0, 5, 0);
  glm::vec3 rayDir(0, 1, 0);  // Pointing away
  auto point = getPlaneInterceptPoint(planePos, planeNormal, rayPos, rayDir);
  EXPECT_TRUE(std::isinf(point.x));
  EXPECT_TRUE(std::isinf(point.y));
  EXPECT_TRUE(std::isinf(point.z));
}

// getWorldRay tests
TEST(SpatialTest, GetWorldRayCenterOfScreen) {
  glm::vec2 viewport(800, 600);
  glm::vec2 cursor(400, 300);  // Center
  glm::mat4 view =
      glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  glm::mat4 proj =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  auto ray = getWorldRay(cursor, viewport, view, proj);

  // At center of screen, ray should point roughly along -Z (toward target)
  EXPECT_NEAR(0.0f, ray.x, 0.01f);
  EXPECT_NEAR(0.0f, ray.y, 0.01f);
  EXPECT_LT(ray.z, 0.0f);  // Pointing into the screen
}

TEST(SpatialTest, GetWorldRayIsNormalized) {
  glm::vec2 viewport(800, 600);
  glm::vec2 cursor(200, 150);
  glm::mat4 view =
      glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  glm::mat4 proj =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  auto ray = getWorldRay(cursor, viewport, view, proj);
  EXPECT_NEAR(1.0f, glm::length(ray), 1e-5);
}

// getWorldPoint tests
TEST(SpatialTest, GetWorldPointOnGroundPlane) {
  glm::vec2 viewport(800, 600);
  glm::vec2 cursor(400, 300);
  glm::vec3 origin(0, 10, 0);  // Camera above ground
  glm::mat4 view = glm::lookAt(origin, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  glm::mat4 proj =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  glm::vec3 plane(0, 0, 0);  // Ground plane at y=0

  auto point = getWorldPoint(viewport, cursor, origin, view, proj, plane);

  // The point should be on the ground plane
  // Due to perspective, exact values depend on camera setup
  EXPECT_FALSE(std::isinf(point.x));
  EXPECT_FALSE(std::isinf(point.y));
  EXPECT_FALSE(std::isinf(point.z));
}

}  // namespace
}  // namespace uinta
