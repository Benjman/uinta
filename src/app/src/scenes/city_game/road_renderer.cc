#include "uinta/scenes/city_game/road_renderer.h"

#include "uinta/math/direction.h"
#include "uinta/mesh.h"

namespace uinta {

RoadRenderer::RoadRenderer(BasicShaderManager* bsm,
                           const OpenGLApi* gl) noexcept
    : vbo_(GL_ARRAY_BUFFER, 0, gl), bsm_(bsm) {
  VaoGuard vaoGuard(&vao_);
  VboGuard vboGuard(&vbo_);
  bsm_->linkAttributes(&vao_);
}

void RoadRenderer::addRoad(const Road& road) noexcept {
  // Create plane mesh at road position
  glm::mat4 transform(1);
  transform = glm::translate(transform, glm::vec3(road.position.x + 0.05f, 0.0f,
                                                  road.position.y - 0.05f));
  transform = glm::rotate(transform, glm::radians<f32>(-90.0), WorldRight);
  transform = glm::scale(transform, glm::vec3(0.9f, 0.9f, 1.0f));

  auto mesh = Mesh::Plane(&idxOffset_, transform);

  // Color based on connection count
  auto roadColor = roadConnectionColor(road.connections);
  mesh.color(glm::vec3(roadColor.r, roadColor.g, roadColor.b));

  // Buffer to GPU (same pattern as CityRenderer)
  auto* data = mesh.vertices().data();
  auto bufSize = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
  VboGuard vbg(&vbo_);
  vtxSegment_ = vbo_.bufferData(data, bufSize, GL_STATIC_DRAW, vtxSegment_);

  VaoGuard vag(&vao_);
  idxSegment_ = vao_.ebo(mesh.elements(), idxSegment_);
  indexCount_ += mesh.elements().size();
  bsm_->linkAttributes(&vao_);
}

void RoadRenderer::render() const noexcept {
  if (empty()) return;

  DepthTestGuard dtg;
  CullFaceGuard cfg;
  ShaderGuard sg(bsm_->shader());
  VaoGuard vg(&vao_);

  bsm_->model(glm::mat4(1));
  bsm_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
}

}  // namespace uinta
