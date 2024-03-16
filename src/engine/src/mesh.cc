#include "uinta/mesh.h"

#include <algorithm>
#include <array>
#include <utility>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/trigonometric.hpp"

namespace uinta {

Mesh::Mesh(const std::span<Mesh> collection) noexcept {
  std::for_each(collection.begin(), collection.end(), [&](auto& mesh) {
    vertices_.insert(vertices_.end(), mesh.vertices().begin(),
                     mesh.vertices().end());
    elements_.insert(elements_.end(), mesh.elements().begin(),
                     mesh.elements().end());
  });
}

Mesh::Mesh(const std::span<Vertex> vertices) noexcept {
  vertices_.insert(vertices_.begin(), vertices.begin(), vertices.end());
}

Mesh::Mesh(const Mesh& other) noexcept
    : vertices_(std::move(other.vertices_)),
      elements_(std::move(other.elements_)) {}

Mesh& Mesh::operator=(const Mesh& other) noexcept {
  if (this != &other) {
    vertices_ = std::move(other.vertices_);
    elements_ = std::move(other.elements_);
  }
  return *this;
}

Mesh::Mesh(const Mesh&& other) noexcept
    : vertices_(std::move(other.vertices_)),
      elements_(std::move(other.elements_)) {}

Mesh& Mesh::operator=(const Mesh&& other) noexcept {
  if (this != &other) {
    vertices_ = std::move(other.vertices_);
    elements_ = std::move(other.elements_);
  }
  return *this;
}

void Mesh::recalculateNormals(size_t idxOffset) noexcept {
  std::for_each(vertices_.begin(), vertices_.end(),
                [](auto& v) { v.normal = glm::vec3(0); });
  for (size_t i = 0; i < elements_.size(); i += 3) {
    auto idx0 = elements_.at(i) - idxOffset;
    auto idx1 = elements_.at(i + 1) - idxOffset;
    auto idx2 = elements_.at(i + 2) - idxOffset;

    auto edgeAB = vertices_.at(idx1).position - vertices_.at(idx0).position;
    auto edgeAC = vertices_.at(idx2).position - vertices_.at(idx0).position;

    auto weightedNormal = glm::cross(edgeAB, edgeAC);

    vertices_.at(idx0).normal += weightedNormal;
    vertices_.at(idx1).normal += weightedNormal;
    vertices_.at(idx2).normal += weightedNormal;
  }
  std::for_each(vertices_.begin(), vertices_.end(),
                [](auto& v) { v.normal = glm::normalize(v.normal); });
}

Mesh Mesh::Cube(size_t* idxOffset, const glm::mat4& transform) noexcept {
  assert(idxOffset && "`idxOffset` cannot be null");
  constexpr auto faceCount = 6;
  constexpr std::array<glm::vec3, faceCount> translations = {
      glm::vec3(1, 0, 1),  // right
      glm::vec3(0, 0, 0),  // left
      glm::vec3(0, 1, 1),  // top
      glm::vec3(0, 0, 0),  // bottom
      glm::vec3(0, 0, 1),  // forward
      glm::vec3(1, 0, 0),  // back
  };
  constexpr std::array<f32, faceCount> angles = {
      90,   // right
      270,  // left
      270,  // top
      90,   // bottom
      0,    // forward
      180,  // back
  };
  constexpr std::array<glm::vec3, faceCount> axes = {
      glm::vec3(0, 1, 0),  // right
      glm::vec3(0, 1, 0),  // left
      glm::vec3(1, 0, 0),  // top
      glm::vec3(1, 0, 0),  // bottom
      glm::vec3(0, 1, 0),  // forward
      glm::vec3(0, 1, 0),  // back
  };
  std::array<Mesh, faceCount> faces;
  for (size_t i = 0; i < faceCount; i++) {
    auto& face = faces.at(i) = Plane(idxOffset);
    auto translate = translations.at(i) - glm::vec3(0.5);
    auto& axis = axes.at(i);
    auto& angle = angles.at(i);
    for (auto& vertex : face.vertices()) {
      auto localTransform = glm::translate(transform, translate);
      localTransform = glm::rotate(localTransform, glm::radians(angle), axis);
      vertex.position = localTransform * glm::vec4(vertex.position, 1);
      vertex.normal = glm::normalize(
          glm::inverseTranspose(glm::mat3(localTransform)) * vertex.normal);
    }
  }

  return Mesh(faces);
}

Mesh Mesh::Plane(size_t* idxOffset, const glm::mat4& transform) noexcept {
  assert(idxOffset && "`idxOffset` cannot be null");
  constexpr glm::vec3 normal(0, 0, 1);

  Mesh plane;
  plane.vertices_.reserve(4);
  plane.vertices_.push_back(Vertex({0, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 1, 0}, normal));
  plane.vertices_.push_back(Vertex({0, 1, 0}, normal));

  std::for_each(
      plane.vertices_.begin(), plane.vertices_.end(),
      [transform](auto& vertex) {
        vertex.position = glm::vec3(transform * glm::vec4(vertex.position, 1));
        vertex.normal = glm::normalize(
            glm::inverseTranspose(glm::mat3(transform)) * vertex.normal);
      });

  plane.elements_ = {1, 3, 0, 1, 2, 3};
  plane.elements_.reserve(6);
  std::for_each(plane.elements_.begin(), plane.elements_.end(),
                [idxOffset](auto& i) { i += *idxOffset; });
  *idxOffset += 4;
  return plane;
}

void Mesh::color(const glm::vec3& color) noexcept {
  std::for_each(vertices_.begin(), vertices_.end(),
                [color](auto& vertex) { vertex.color = color; });
}

}  // namespace uinta
