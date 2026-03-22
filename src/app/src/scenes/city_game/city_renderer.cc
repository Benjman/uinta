#include "uinta/scenes/city_game/city_renderer.h"

#include "absl/random/random.h"
#include "uinta/math/direction.h"
#include "uinta/mesh.h"

namespace uinta {

CityRenderer::CityRenderer(BasicShaderManager* bsm,
                           const OpenGLApi* gl) noexcept
    : vbo_(GL_ARRAY_BUFFER, 0, gl), bsm_(bsm) {
  VaoGuard vaoGuard(&vao_);
  VboGuard vboGuard(&vbo_);
  bsm_->linkAttributes(&vao_);
}

void CityRenderer::addBuilding(const Building& building) noexcept {
  f32 size = buildingTypeSize(building.type);
  glm::mat4 transform(1);
  transform = glm::translate(
      transform, glm::vec3(building.position.x, 0.05, building.position.y));
  transform = glm::scale(transform, glm::vec3(size, 0.1, size));
  transform =
      glm::rotate(transform, glm::radians<f32>(absl::BitGen()()), WorldUp);
  auto mesh = Mesh::Cube(&idxOffset_, transform);

  auto buildingColor = buildingTypeColor(building.type);
  mesh.color(glm::vec3(buildingColor.r, buildingColor.g, buildingColor.b));

  auto* data = mesh.vertices().data();
  auto bufSize = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
  VboGuard vbg(&vbo_);
  vtxSegment_ = vbo_.bufferData(data, bufSize, GL_STATIC_DRAW, vtxSegment_);

  VaoGuard vag(&vao_);
  idxSegment_ = vao_.ebo(mesh.elements(), idxSegment_);
  indexCount_ += mesh.elements().size();
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
