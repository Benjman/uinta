#include "uinta/scenes/city_game/intersection_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "uinta/mesh.h"
#include "uinta/scenes/city_game/building_types.h"

namespace uinta {

IntersectionRenderer::IntersectionRenderer(BasicShaderManager* bsm,
                                           const OpenGLApi* gl) noexcept
    : buildingVbo_(GL_ARRAY_BUFFER, 0, gl),
      groundVbo_(GL_ARRAY_BUFFER, 0, gl),
      bsm_(bsm) {
  // Initialize building VAO
  {
    VaoGuard vaoGuard(&buildingVao_);
    VboGuard vboGuard(&buildingVbo_);
    bsm_->linkAttributes(&buildingVao_);
  }

  // Initialize ground VAO
  {
    VaoGuard vaoGuard(&groundVao_);
    VboGuard vboGuard(&groundVbo_);
    bsm_->linkAttributes(&groundVao_);
  }

  initGroundMesh();
}

void IntersectionRenderer::initGroundMesh() noexcept {
  // Create ground plane (large flat square at y=0)
  constexpr f32 groundSize = 50.0f;
  constexpr f32 halfSize = groundSize / 2.0f;

  // Ground plane vertices
  std::vector<Vertex> vertices;
  vertices.reserve(4);

  glm::vec3 groundColor = glm::vec3(color::Gray800);
  glm::vec3 normal(0, 1, 0);

  vertices.push_back(Vertex{.position = {-halfSize, 0, -halfSize},
                            .normal = normal,
                            .color = groundColor});
  vertices.push_back(Vertex{.position = {halfSize, 0, -halfSize},
                            .normal = normal,
                            .color = groundColor});
  vertices.push_back(Vertex{.position = {halfSize, 0, halfSize},
                            .normal = normal,
                            .color = groundColor});
  vertices.push_back(Vertex{.position = {-halfSize, 0, halfSize},
                            .normal = normal,
                            .color = groundColor});

  std::vector<idx_t> elements = {0, 1, 2, 0, 2, 3};

  VboGuard vbg(&groundVbo_);
  auto bufSize = vertices.size() * Vertex::ElementCount * sizeof(f32);
  groundVtxSegment_ = groundVbo_.bufferData(vertices.data(), bufSize,
                                            GL_STATIC_DRAW, groundVtxSegment_);

  VaoGuard vag(&groundVao_);
  groundIdxSegment_ = groundVao_.ebo(elements, groundIdxSegment_);
  groundIndexCount_ = elements.size();
  bsm_->linkAttributes(&groundVao_);
}

void IntersectionRenderer::buildMesh(
    const IntersectionBuildings& buildings) noexcept {
  if (buildings.empty()) return;

  idx_t idxOffset = 0;
  std::vector<Mesh> meshes;
  meshes.reserve(buildings.count());

  for (size_t i = 0; i < buildings.count(); ++i) {
    glm::vec3 pos = buildings.positions[i];
    glm::vec3 dims = buildings.dimensions[i];
    BuildingType type = buildings.types[i];

    // Create a cube mesh, transform it to building position/size
    auto mesh = Mesh::Cube(&idxOffset);

    // Set color based on building type
    glm::vec3 col = glm::vec3(buildingTypeColor(type));
    mesh.color(col);

    // Scale to dimensions (cube is 1x1x1 centered at origin)
    mesh.scale(dims);

    // Translate: pivot at base center
    // The cube is centered, so we need to move it up by half height
    mesh.translate(glm::vec3(pos.x, dims.y / 2.0f, pos.z));

    meshes.push_back(mesh);
  }

  // Merge all meshes
  Mesh combined(meshes);

  VboGuard vbg(&buildingVbo_);
  auto bufSize =
      combined.vertices().size() * Vertex::ElementCount * sizeof(f32);
  buildingVtxSegment_ = buildingVbo_.bufferData(
      combined.vertices().data(), bufSize, GL_STATIC_DRAW, buildingVtxSegment_);

  VaoGuard vag(&buildingVao_);
  buildingIdxSegment_ =
      buildingVao_.ebo(combined.elements(), buildingIdxSegment_);
  buildingIndexCount_ = combined.elements().size();
  bsm_->linkAttributes(&buildingVao_);
}

void IntersectionRenderer::render() const noexcept {
  DepthTestGuard dtg;
  CullFaceGuard cfg;
  ShaderGuard sg(bsm_->shader());

  // Render ground
  if (groundIndexCount_ > 0) {
    VaoGuard vg(&groundVao_);
    bsm_->model(glm::mat4(1.0f));
    bsm_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(groundIndexCount_),
                       GL_UNSIGNED_INT, nullptr);
  }

  // Render buildings
  if (buildingIndexCount_ > 0) {
    VaoGuard vg(&buildingVao_);
    bsm_->model(glm::mat4(1.0f));
    bsm_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(buildingIndexCount_),
                       GL_UNSIGNED_INT, nullptr);
  }
}

}  // namespace uinta
