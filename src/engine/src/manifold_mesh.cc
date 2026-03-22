#include "uinta/manifold_mesh.h"

#include <cstdint>
#include <type_traits>
#include <vector>

#include "uinta/mesh.h"
#include "uinta/types.h"

namespace uinta {

namespace {

// Manifold's default `MeshGL::I` is `uint32_t`. `uinta::idx_t` must be wide
// enough to hold every value Manifold will produce.
static_assert(sizeof(idx_t) >= sizeof(uint32_t),
              "uinta::idx_t must be at least 32-bit to hold manifold indices");
static_assert(std::is_unsigned_v<idx_t>,
              "uinta::idx_t is expected to be an unsigned integer type");

// Manifold::CalculateNormals(n, ...) reserves `n` user property channels
// *after* the 3 position channels and writes the 3 normal channels after
// that. So CalculateNormals(0) gives layout pos(0..2), normal(3..5), with
// numProp == 6. This is the minimal layout we need since this converter
// does not propagate user color/UV channels through Manifold.
constexpr int NormalIdx = 0;
constexpr size_t PositionOffset = 0;
constexpr size_t NormalOffset = 3;
constexpr uint32_t ExpectedNumProp = 6;

}  // namespace

Mesh ToMesh(const manifold::Manifold& m, double minSharpAngle) noexcept {
  // Short-circuit empty manifolds without touching the Manifold CSG pipeline.
  if (m.IsEmpty()) {
    return Mesh{};
  }

  const manifold::Manifold withNormals =
      m.CalculateNormals(NormalIdx, minSharpAngle);
  // Intentionally do NOT pass `NormalIdx` to GetMeshGL. On composed Boolean
  // results Manifold's transform-correction pass (triggered by providing the
  // normalIdx argument) flips the sign of a sizable fraction of the per-vertex
  // normals relative to the raw values CalculateNormals produced, causing
  // visibly inverted lighting in the rendered mesh. The no-arg form returns
  // the normals as computed, matching the library's CCW-from-outside winding.
  const manifold::MeshGL mesh = withNormals.GetMeshGL();

  const size_t numVerts = mesh.NumVert();
  const uint32_t numProp = mesh.numProp;

  std::vector<Vertex> vertices;
  vertices.reserve(numVerts);

  // Guard: if CalculateNormals didn't give us at least position+normal
  // channels we fall back to leaving normals at their default. In practice
  // CalculateNormals always extends numProp to at least kExpectedNumProp.
  const bool hasNormals = numProp >= ExpectedNumProp;

  for (size_t v = 0; v < numVerts; ++v) {
    const size_t offset = v * numProp;

    Vertex::position_type position{
        mesh.vertProperties[offset + PositionOffset + 0],
        mesh.vertProperties[offset + PositionOffset + 1],
        mesh.vertProperties[offset + PositionOffset + 2]};

    Vertex::normal_type normal{};
    if (hasNormals) {
      normal =
          Vertex::normal_type{mesh.vertProperties[offset + NormalOffset + 0],
                              mesh.vertProperties[offset + NormalOffset + 1],
                              mesh.vertProperties[offset + NormalOffset + 2]};
    }

    vertices.emplace_back(Vertex{.position = position, .normal = normal});
  }

  std::vector<idx_t> elements;
  elements.reserve(mesh.triVerts.size());
  for (const uint32_t triVert : mesh.triVerts) {
    elements.push_back(static_cast<idx_t>(triVert));
  }

  return Mesh{vertices, elements};
}

}  // namespace uinta
