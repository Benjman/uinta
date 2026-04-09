#include "uinta/mesh.h"

#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

namespace uinta {
namespace {

constexpr float kEpsilon = 1e-5f;

bool Vec3NearEqual(const glm::vec3& a, const glm::vec3& b,
                   float epsilon = kEpsilon) {
  return std::abs(a.x - b.x) < epsilon && std::abs(a.y - b.y) < epsilon &&
         std::abs(a.z - b.z) < epsilon;
}

// Vertex tests
struct VertexTest : public testing::Test {};

TEST_F(VertexTest, ElementCountIsCorrect) {
  // position(3) + normal(3) + color(3) + uv(2) = 11
  ASSERT_EQ(11u, Vertex::ElementCount);
}

TEST_F(VertexTest, DefaultNormalIsZero) {
  Vertex v{.position = {0, 0, 0}};
  ASSERT_EQ(glm::vec3(0, 0, 0), v.normal);
}

TEST_F(VertexTest, DefaultColorIsWhite) {
  Vertex v{.position = {0, 0, 0}};
  ASSERT_EQ(glm::vec3(1, 1, 1), v.color);
}

TEST_F(VertexTest, DefaultUvIsZero) {
  Vertex v{.position = {0, 0, 0}};
  ASSERT_EQ(glm::vec2(0, 0), v.uv);
}

// Mesh tests
struct MeshTest : public testing::Test {};

TEST_F(MeshTest, DefaultConstructorCreatesEmptyMesh) {
  Mesh mesh;
  ASSERT_TRUE(mesh.vertices().empty());
  ASSERT_TRUE(mesh.elements().empty());
}

TEST_F(MeshTest, ConstructorFromVertexSpan) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
      Vertex{.position = {0, 1, 0}},
  };
  std::span<const Vertex> span(vertices);
  Mesh mesh(span);
  ASSERT_EQ(3u, mesh.vertices().size());
  ASSERT_TRUE(mesh.elements().empty());
}

TEST_F(MeshTest, ConstructorFromVerticesAndElements) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
      Vertex{.position = {0, 1, 0}},
  };
  std::vector<idx_t> elements = {0, 1, 2};
  Mesh mesh(vertices, elements);
  ASSERT_EQ(3u, mesh.vertices().size());
  ASSERT_EQ(3u, mesh.elements().size());
}

TEST_F(MeshTest, ConstructorFromMeshSpan) {
  std::vector<Vertex> v1 = {Vertex{.position = {0, 0, 0}}};
  std::vector<Vertex> v2 = {Vertex{.position = {1, 1, 1}}};
  std::vector<idx_t> e1 = {0};
  std::vector<idx_t> e2 = {1};

  Mesh mesh1(v1, e1);
  Mesh mesh2(v2, e2);
  std::array<Mesh, 2> meshes = {mesh1, mesh2};
  std::span<const Mesh> span(meshes);

  Mesh combined(span);
  ASSERT_EQ(2u, combined.vertices().size());
  ASSERT_EQ(2u, combined.elements().size());
}

TEST_F(MeshTest, CopyConstructor) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh original(vertices, elements);

  Mesh copy(original);
  ASSERT_EQ(1u, copy.vertices().size());
  ASSERT_EQ(1u, copy.elements().size());
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(1, 2, 3), copy.vertices()[0].position));
}

TEST_F(MeshTest, MoveConstructor) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh original(vertices, elements);

  Mesh moved(std::move(original));
  ASSERT_EQ(1u, moved.vertices().size());
  ASSERT_EQ(1u, moved.elements().size());
}

TEST_F(MeshTest, CopyAssignment) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh original(vertices, elements);

  Mesh copy;
  copy = original;
  ASSERT_EQ(1u, copy.vertices().size());
  ASSERT_EQ(1u, copy.elements().size());
}

TEST_F(MeshTest, MoveAssignment) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh original(vertices, elements);

  Mesh moved;
  moved = std::move(original);
  ASSERT_EQ(1u, moved.vertices().size());
  ASSERT_EQ(1u, moved.elements().size());
}

TEST_F(MeshTest, SelfCopyAssignment) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh mesh(vertices, elements);

  Mesh* meshPtr = &mesh;
  mesh = *meshPtr;  // Self-assignment via pointer
  ASSERT_EQ(1u, mesh.vertices().size());
  ASSERT_EQ(1u, mesh.elements().size());
}

TEST_F(MeshTest, SelfMoveAssignment) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  std::vector<idx_t> elements = {0};
  Mesh mesh(vertices, elements);

  Mesh* meshPtr = &mesh;
  mesh = std::move(*meshPtr);  // Self-move-assignment via pointer
  ASSERT_EQ(1u, mesh.vertices().size());
  ASSERT_EQ(1u, mesh.elements().size());
}

TEST_F(MeshTest, MergeSingleMesh) {
  std::vector<Vertex> v1 = {Vertex{.position = {0, 0, 0}}};
  std::vector<Vertex> v2 = {Vertex{.position = {1, 1, 1}}};
  std::vector<idx_t> e1 = {0};
  std::vector<idx_t> e2 = {1};

  Mesh mesh1(v1, e1);
  Mesh mesh2(v2, e2);

  mesh1.merge(mesh2);
  ASSERT_EQ(2u, mesh1.vertices().size());
  ASSERT_EQ(2u, mesh1.elements().size());
}

TEST_F(MeshTest, MergeMultipleMeshes) {
  std::vector<Vertex> v1 = {Vertex{.position = {0, 0, 0}}};
  std::vector<Vertex> v2 = {Vertex{.position = {1, 1, 1}}};
  std::vector<Vertex> v3 = {Vertex{.position = {2, 2, 2}}};

  Mesh mesh1(v1, std::vector<idx_t>{0});
  Mesh mesh2(v2, std::vector<idx_t>{1});
  Mesh mesh3(v3, std::vector<idx_t>{2});

  std::array<Mesh, 2> toMerge = {mesh2, mesh3};
  mesh1.merge(std::span<const Mesh>(toMerge));

  ASSERT_EQ(3u, mesh1.vertices().size());
  ASSERT_EQ(3u, mesh1.elements().size());
}

TEST_F(MeshTest, AddVertex) {
  Mesh mesh;
  Vertex v{.position = {1, 2, 3}};
  mesh.addVertex(v);
  ASSERT_EQ(1u, mesh.vertices().size());
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(1, 2, 3), mesh.vertices()[0].position));
}

TEST_F(MeshTest, VerticesConstAccessor) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  const Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  const auto& verts = mesh.vertices();
  ASSERT_EQ(1u, verts.size());
}

TEST_F(MeshTest, VerticesMutableAccessor) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  auto verts = mesh.vertices();
  verts[0].position = glm::vec3(4, 5, 6);
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(4, 5, 6), mesh.vertices()[0].position));
}

TEST_F(MeshTest, VerticescReturnsConstSpan) {
  std::vector<Vertex> vertices = {Vertex{.position = {1, 2, 3}}};
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  std::span<const Vertex> verts = mesh.verticesc();
  ASSERT_EQ(1u, verts.size());
}

TEST_F(MeshTest, ElementsConstAccessor) {
  std::vector<Vertex> vertices = {Vertex{.position = {0, 0, 0}}};
  std::vector<idx_t> elements = {0, 1, 2};
  const Mesh mesh(vertices, elements);
  const auto& elems = mesh.elements();
  ASSERT_EQ(3u, elems.size());
}

TEST_F(MeshTest, ElementsMutableAccessor) {
  std::vector<Vertex> vertices = {Vertex{.position = {0, 0, 0}}};
  std::vector<idx_t> elements = {0, 1, 2};
  Mesh mesh(vertices, elements);
  auto elems = mesh.elements();
  elems[0] = 99;
  ASSERT_EQ(99u, mesh.elements()[0]);
}

TEST_F(MeshTest, ElementscReturnsConstSpan) {
  std::vector<Vertex> vertices = {Vertex{.position = {0, 0, 0}}};
  std::vector<idx_t> elements = {0, 1, 2};
  Mesh mesh(vertices, elements);
  std::span<const idx_t> elems = mesh.elementsc();
  ASSERT_EQ(3u, elems.size());
}

TEST_F(MeshTest, ReserveIncreasesCapacity) {
  Mesh mesh;
  mesh.reserve(100, 200);
  // Add a vertex to verify the reserve worked (mesh should not need to
  // reallocate)
  for (size_t i = 0; i < 100; ++i) {
    mesh.addVertex(Vertex{.position = {0, 0, 0}});
  }
  ASSERT_EQ(100u, mesh.vertices().size());
}

TEST_F(MeshTest, ColorSetsAllVertices) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
      Vertex{.position = {0, 1, 0}},
  };
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  glm::vec3 newColor(0.5f, 0.6f, 0.7f);
  mesh.color(newColor);

  for (const auto& v : mesh.vertices()) {
    ASSERT_TRUE(Vec3NearEqual(newColor, v.color));
  }
}

TEST_F(MeshTest, TranslateMovesAllPositions) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
  };
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  mesh.translate(glm::vec3(10, 20, 30));

  ASSERT_TRUE(
      Vec3NearEqual(glm::vec3(10, 20, 30), mesh.vertices()[0].position));
  ASSERT_TRUE(
      Vec3NearEqual(glm::vec3(11, 20, 30), mesh.vertices()[1].position));
}

TEST_F(MeshTest, ScaleScalesAllPositions) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {1, 2, 3}},
      Vertex{.position = {2, 4, 6}},
  };
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});
  mesh.scale(glm::vec3(2, 2, 2));

  ASSERT_TRUE(Vec3NearEqual(glm::vec3(2, 4, 6), mesh.vertices()[0].position));
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(4, 8, 12), mesh.vertices()[1].position));
}

TEST_F(MeshTest, RotateRotatesPositions) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {1, 0, 0}},
  };
  std::vector<idx_t> elements = {0, 0, 0};  // Dummy elements for normal calc
  Mesh mesh(vertices, elements);

  mesh.rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));

  // After 90 degree rotation around Y axis: (1,0,0) -> (0,0,-1)
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(0, 0, -1), mesh.vertices()[0].position));
}

TEST_F(MeshTest, TransformPositionsAppliesMatrix) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {1, 0, 0}},
  };
  Mesh mesh(std::span<const Vertex>(vertices), std::vector<idx_t>{});

  glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(5, 5, 5));
  mesh.transformPositions(transform);

  ASSERT_TRUE(Vec3NearEqual(glm::vec3(6, 5, 5), mesh.vertices()[0].position));
}

TEST_F(MeshTest, RecalculateNormalsBasicTriangle) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
      Vertex{.position = {0, 1, 0}},
  };
  std::vector<idx_t> elements = {0, 1, 2};
  Mesh mesh(vertices, elements);

  mesh.recalculateNormals();

  // For a triangle in XY plane, normal should point in Z direction
  for (const auto& v : mesh.vertices()) {
    ASSERT_NEAR(0.0f, v.normal.x, kEpsilon);
    ASSERT_NEAR(0.0f, v.normal.y, kEpsilon);
    // Normal direction depends on winding order
    ASSERT_NEAR(1.0f, std::abs(v.normal.z), kEpsilon);
  }
}

TEST_F(MeshTest, RecalculateNormalsWithOffset) {
  std::vector<Vertex> vertices = {
      Vertex{.position = {0, 0, 0}},
      Vertex{.position = {1, 0, 0}},
      Vertex{.position = {0, 1, 0}},
  };
  // Elements with offset of 10 (as if these vertices start at index 10)
  std::vector<idx_t> elements = {10, 11, 12};
  Mesh mesh(vertices, elements);

  mesh.recalculateNormals(10);

  for (const auto& v : mesh.vertices()) {
    ASSERT_NEAR(1.0f, std::abs(v.normal.z), kEpsilon);
  }
}

// Plane factory tests
struct PlaneTest : public testing::Test {};

TEST_F(PlaneTest, CreatesPlaneMesh) {
  Mesh plane = Mesh::Plane();
  ASSERT_EQ(4u, plane.vertices().size());
  ASSERT_EQ(6u, plane.elements().size());  // 2 triangles = 6 indices
}

TEST_F(PlaneTest, PlaneWithIdxOffset) {
  idx_t offset = 10;
  Mesh plane = Mesh::Plane(&offset);

  ASSERT_EQ(14u, offset);  // Initial 10 + 4 vertices

  // Elements should be offset by 10
  for (const auto& elem : plane.elements()) {
    ASSERT_GE(elem, 10u);
    ASSERT_LT(elem, 14u);
  }
}

TEST_F(PlaneTest, PlaneWithTransform) {
  glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(5, 5, 5));
  Mesh plane = Mesh::Plane(nullptr, transform);

  // All vertices should be offset by (5, 5, 5)
  for (const auto& v : plane.vertices()) {
    ASSERT_GE(v.position.x, 5.0f - kEpsilon);
    ASSERT_GE(v.position.y, 5.0f - kEpsilon);
    ASSERT_GE(v.position.z, 5.0f - kEpsilon);
  }
}

TEST_F(PlaneTest, PlaneHasCorrectNormals) {
  Mesh plane = Mesh::Plane();

  // Default plane should have normals pointing in +Z direction
  for (const auto& v : plane.vertices()) {
    ASSERT_TRUE(Vec3NearEqual(glm::vec3(0, 0, 1), v.normal));
  }
}

// Cube factory tests
struct CubeTest : public testing::Test {};

TEST_F(CubeTest, CreatesCubeMesh) {
  Mesh cube = Mesh::Cube();
  // 6 faces * 4 vertices = 24 vertices
  ASSERT_EQ(24u, cube.vertices().size());
  // 6 faces * 2 triangles * 3 indices = 36 elements
  ASSERT_EQ(36u, cube.elements().size());
}

TEST_F(CubeTest, CubeWithIdxOffset) {
  idx_t offset = 0;
  Mesh cube = Mesh::Cube(&offset);

  ASSERT_EQ(24u, offset);  // 6 faces * 4 vertices
}

TEST_F(CubeTest, CubeWithTransform) {
  glm::mat4 transform = glm::scale(glm::mat4(1), glm::vec3(2, 2, 2));
  Mesh cube = Mesh::Cube(nullptr, transform);

  // Find min/max extents
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();
  for (const auto& v : cube.vertices()) {
    minX = std::min(minX, v.position.x);
    maxX = std::max(maxX, v.position.x);
  }

  // Scaled cube should be 2x larger
  ASSERT_NEAR(2.0f, maxX - minX, kEpsilon);
}

TEST_F(CubeTest, CubeIsCentered) {
  Mesh cube = Mesh::Cube();

  glm::vec3 center(0);
  for (const auto& v : cube.vertices()) {
    center += v.position;
  }
  center /= static_cast<float>(cube.vertices().size());

  // Cube should be centered at origin
  ASSERT_TRUE(Vec3NearEqual(glm::vec3(0, 0, 0), center));
}

TEST_F(CubeTest, CubeHasNormalizedNormals) {
  Mesh cube = Mesh::Cube();

  for (const auto& v : cube.vertices()) {
    float length = glm::length(v.normal);
    ASSERT_NEAR(1.0f, length, kEpsilon);
  }
}

TEST_F(CubeTest, MultipleIdxOffsetCalls) {
  idx_t offset = 0;

  Mesh cube1 = Mesh::Cube(&offset);
  ASSERT_EQ(24u, offset);

  Mesh cube2 = Mesh::Cube(&offset);
  ASSERT_EQ(48u, offset);

  // Second cube's elements should start at 24
  bool hasHigherIndices = false;
  for (const auto& elem : cube2.elements()) {
    if (elem >= 24) {
      hasHigherIndices = true;
    }
  }
  ASSERT_TRUE(hasHigherIndices);
}

}  // namespace
}  // namespace uinta
