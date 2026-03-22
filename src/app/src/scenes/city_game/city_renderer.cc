#include "uinta/scenes/city_game/city_renderer.h"

#include <absl/random/random.h>
#include <absl/strings/str_format.h>

#include <optional>

#include "uinta/math/direction.h"
#include "uinta/mesh.h"

namespace uinta {

Mesh generateHouseMesh(Color color, const glm::mat4& transform) noexcept {
  using namespace manifold;

  // house dimensions
  const double width = 0.6;   // X dimension
  const double depth = 0.8;   // Z dimension
  const double height = 0.6;  // Y dimension
  const double roofHeight = 0.4;

  // Create cube base, centered at origin
  Manifold base = Manifold::Cube({width, height, depth}, true);
  base = base.Translate({0.0, height / 2.0, 0.0});

  // Create pitched roof (triangle profile extruded along Z)
  Polygons roofProfile = {{
      {-width / 2.0, 0.0},
      {width / 2.0, 0.0},
      {0.0, roofHeight},
  }};
  Manifold roof = Manifold::Extrude(roofProfile, depth, 0, 0.0, {0.0, 0.0});
  roof = roof.Translate({0.0, height, -depth / 2.0});

  // Combine and calculate normals
  Manifold house = (base + roof).CalculateNormals(0, 60.0);
  MeshGL mMesh = house.GetMeshGL();

  // Convert to uinta::Mesh
  std::vector<Vertex> vertices;
  vertices.reserve(mMesh.NumVert());

  for (size_t i = 0; i < mMesh.NumVert(); ++i) {
    size_t offset = i * mMesh.numProp;
    glm::vec3 position{static_cast<float>(mMesh.vertProperties[offset]),
                       static_cast<float>(mMesh.vertProperties[offset + 1]),
                       static_cast<float>(mMesh.vertProperties[offset + 2])};

    glm::vec3 normal{0.0f, 1.0f, 0.0f};
    if (mMesh.numProp >= 6) {
      normal = {static_cast<float>(mMesh.vertProperties[offset + 3]),
                static_cast<float>(mMesh.vertProperties[offset + 4]),
                static_cast<float>(mMesh.vertProperties[offset + 5])};
    }

    vertices.emplace_back(Vertex{.position = position,
                                 .normal = normal,
                                 .color = color,
                                 .uv = Vertex::uv_type{0.0f, 0.0f}});
  }

  std::vector<idx_t> elements;
  elements.reserve(mMesh.triVerts.size());
  for (size_t i = 0; i < mMesh.triVerts.size(); ++i) {
    elements.push_back(static_cast<idx_t>(mMesh.triVerts[i]));
  }

  // Apply transform to vertices before creating mesh
  glm::vec4 transformedPos;
  for (auto& vertex : vertices) {
    transformedPos = transform * glm::vec4(vertex.position, 1.0f);
    vertex.position = glm::vec3(transformedPos);
  }

  return Mesh(vertices, elements);
}

CityRenderer::CityRenderer(BasicShaderManager* bsm,
                           const OpenGLApi* gl) noexcept
    : vbo_(GL_ARRAY_BUFFER, 0, gl), bsm_(bsm) {
  VaoGuard vaoGuard(&vao_);
  VboGuard vboGuard(&vbo_);
  bsm_->linkAttributes(&vao_);
}

void CityRenderer::addBuilding(const Building& building) noexcept {
  static auto bitgen = absl::BitGen();
  glm::mat4 transform = glm::translate(
      glm::mat4(1), glm::vec3(building.position.x, 0.00, building.position.y));
  f32 randomAngle =
      glm::radians<f32>(static_cast<f32>(absl::Uniform(bitgen, 0.0, 360.0)));
  transform = glm::rotate(transform, randomAngle, WorldUp);
  auto mesh = generateHouseMesh(buildingTypeColor(building.type), transform);

  auto* data = mesh.vertices().data();
  auto bufSize = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
  VboGuard vbg(&vbo_);
  vtxSegment_ = vbo_.bufferData(data, bufSize, GL_STATIC_DRAW, vtxSegment_);

  // Offset indices by the number of vertices already in the buffer
  idx_t vertexOffset = idxOffset_;
  std::vector<u32> offsetIndices;
  offsetIndices.reserve(mesh.elements().size());
  for (auto idx : mesh.elements()) {
    offsetIndices.push_back(idx + vertexOffset);
  }

  VaoGuard vag(&vao_);
  idxSegment_ = vao_.ebo(offsetIndices, idxSegment_);
  indexCount_ += mesh.elements().size();
  idxOffset_ += mesh.vertices().size();
  bsm_->linkAttributes(&vao_);
}

void CityRenderer::render() const noexcept {
  if (empty()) return;

  DepthTestGuard dtg;
  CullFaceGuard cfg;
  ShaderGuard sg(bsm_->shader());
  VaoGuard vg(&vao_);

  bsm_->model(glm::scale(glm::mat4(1), glm::vec3(1)));
  bsm_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
}

}  // namespace uinta
