#include "uinta/manifold_mesh.h"

#include <gtest/gtest.h>
#include <manifold/manifold.h>

#include <algorithm>
#include <cmath>
#include <set>
#include <tuple>

#include "glm/geometric.hpp"
#include "uinta/mesh.h"

namespace uinta {
namespace {

constexpr float kEpsilon = 1e-4f;

TEST(ManifoldMeshTest, EmptyManifoldProducesEmptyMesh) {
  manifold::Manifold empty;
  ASSERT_TRUE(empty.IsEmpty());

  Mesh mesh = ToMesh(empty);
  ASSERT_TRUE(mesh.vertices().empty());
  ASSERT_TRUE(mesh.elements().empty());
}

TEST(ManifoldMeshTest, CubeHasTriangulatedFaces) {
  auto cube = manifold::Manifold::Cube({1.0, 1.0, 1.0}, /*center=*/true);
  Mesh mesh = ToMesh(cube);

  // Non-empty, indexed, and element count divisible by 3.
  ASSERT_FALSE(mesh.vertices().empty());
  ASSERT_FALSE(mesh.elements().empty());
  ASSERT_EQ(0u, mesh.elements().size() % 3);

  // All positions sit on the ±0.5 cube.
  for (const auto& v : mesh.vertices()) {
    ASSERT_NEAR(0.5f,
                std::max({std::abs(v.position.x), std::abs(v.position.y),
                          std::abs(v.position.z)}),
                kEpsilon);
  }
}

TEST(ManifoldMeshTest, CubeIndicesAreInRange) {
  auto cube = manifold::Manifold::Cube({1.0, 1.0, 1.0}, true);
  Mesh mesh = ToMesh(cube);

  const auto numVerts = mesh.vertices().size();
  for (const auto idx : mesh.elements()) {
    ASSERT_LT(idx, numVerts);
  }
}

TEST(ManifoldMeshTest, CubeNormalsAreAxisAlignedAtDefaultSharpness) {
  // Default minSharpAngle=60 keeps cube faces faceted (90° dihedral > 60°),
  // so every vertex normal should be ±X, ±Y, or ±Z.
  auto cube = manifold::Manifold::Cube({1.0, 1.0, 1.0}, true);
  Mesh mesh = ToMesh(cube);

  for (const auto& v : mesh.vertices()) {
    const float len = glm::length(v.normal);
    ASSERT_NEAR(1.0f, len, kEpsilon);

    const float ax = std::abs(v.normal.x);
    const float ay = std::abs(v.normal.y);
    const float az = std::abs(v.normal.z);
    const float maxComp = std::max({ax, ay, az});
    ASSERT_NEAR(1.0f, maxComp, kEpsilon);
  }

  // There should be exactly 6 unique normals (±X, ±Y, ±Z).
  std::set<std::tuple<int, int, int>> uniqueNormals;
  for (const auto& v : mesh.vertices()) {
    uniqueNormals.emplace(static_cast<int>(std::round(v.normal.x)),
                          static_cast<int>(std::round(v.normal.y)),
                          static_cast<int>(std::round(v.normal.z)));
  }
  ASSERT_EQ(6u, uniqueNormals.size());
}

TEST(ManifoldMeshTest, SphereNormalsPointOutward) {
  auto sphere = manifold::Manifold::Sphere(1.0, 32);
  Mesh mesh = ToMesh(sphere);

  ASSERT_FALSE(mesh.vertices().empty());
  ASSERT_FALSE(mesh.elements().empty());

  for (const auto& v : mesh.vertices()) {
    // Positions lie on the unit sphere.
    ASSERT_NEAR(1.0f, glm::length(v.position), 1e-2f);

    // Normals are unit length.
    ASSERT_NEAR(1.0f, glm::length(v.normal), kEpsilon);

    // For a sphere the outward normal should agree with the position direction.
    const float dot = glm::dot(glm::normalize(v.position), v.normal);
    ASSERT_GT(dot, 0.95f);
  }
}

TEST(ManifoldMeshTest, DefaultVertexColorAndUvArePreserved) {
  auto cube = manifold::Manifold::Cube({1.0, 1.0, 1.0}, true);
  Mesh mesh = ToMesh(cube);

  for (const auto& v : mesh.vertices()) {
    ASSERT_EQ(glm::vec3(1, 1, 1), v.color);
    ASSERT_EQ(glm::vec2(0, 0), v.uv);
  }
}

TEST(ManifoldMeshTest, BooleanOperationsProduceValidMesh) {
  // Classic CSG sanity check: sphere minus cube.
  auto sphere = manifold::Manifold::Sphere(1.0, 24);
  auto cube = manifold::Manifold::Cube({1.2, 1.2, 1.2}, true);
  auto result = sphere - cube;

  Mesh mesh = ToMesh(result);
  ASSERT_FALSE(mesh.vertices().empty());
  ASSERT_FALSE(mesh.elements().empty());
  ASSERT_EQ(0u, mesh.elements().size() % 3);

  const auto numVerts = mesh.vertices().size();
  for (const auto idx : mesh.elements()) {
    ASSERT_LT(idx, numVerts);
  }
  for (const auto& v : mesh.vertices()) {
    ASSERT_NEAR(1.0f, glm::length(v.normal), kEpsilon);
  }
}

}  // namespace
}  // namespace uinta
