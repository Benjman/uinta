#include "uinta/primitive.h"

#include <algorithm>
#include <array>
#include <utility>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/trigonometric.hpp"

namespace uinta {

Primitive::Primitive(const std::span<Primitive> collection) noexcept {
  std::for_each(collection.begin(), collection.end(), [&](auto& primitive) {
    vertices_.insert(vertices_.end(), primitive.vertices().begin(),
                     primitive.vertices().end());
    elements_.insert(elements_.end(), primitive.elements().begin(),
                     primitive.elements().end());
  });
}

Primitive::Primitive(const Primitive& other) noexcept
    : vertices_(std::move(other.vertices_)),
      elements_(std::move(other.elements_)) {}

Primitive& Primitive::operator=(const Primitive& other) noexcept {
  if (this != &other) {
    vertices_ = std::move(other.vertices_);
    elements_ = std::move(other.elements_);
  }
  return *this;
}

Primitive::Primitive(const Primitive&& other) noexcept
    : vertices_(std::move(other.vertices_)),
      elements_(std::move(other.elements_)) {}

Primitive& Primitive::operator=(const Primitive&& other) noexcept {
  if (this != &other) {
    vertices_ = std::move(other.vertices_);
    elements_ = std::move(other.elements_);
  }
  return *this;
}

void Primitive::recalculateNormals(size_t idxOffset) noexcept {
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

void transformVertex(Vertex* vertex, const glm::vec3& translate, f32 rotAngle,
                     const glm::vec3& rotAxis) noexcept {
  auto transform = glm::translate(glm::mat4(1), translate);
  transform = glm::rotate(transform, glm::radians(rotAngle), rotAxis);
  vertex->position = transform * glm::vec4(vertex->position, 1);
  vertex->normal = glm::rotate(glm::mat4(1), glm::radians(rotAngle), rotAxis) *
                   glm::vec4(vertex->normal, 1);
}

Primitive Primitive::Cube(size_t* idxOffset) noexcept {
  constexpr size_t faceCount = 6;
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
      -90,  // left
      -90,  // top
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
  std::array<Primitive, faceCount> faces;
  for (size_t i = 0; i < faceCount; i++) {
    auto& face = faces.at(i) = Plane(idxOffset);
    auto translation = translations.at(i) - glm::vec3(0.5);
    auto& axis = axes.at(i);
    auto& angle = angles.at(i);
    for (auto& vertex : face.vertices())
      transformVertex(&vertex, translation, angle, axis);
  }

  return Primitive(faces);
}

Primitive Primitive::Plane(size_t* idxOffset) noexcept {
  constexpr glm::vec3 normal(0, 0, 1);

  Primitive plane;
  plane.vertices_.reserve(4);
  plane.vertices_.push_back(Vertex({0, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 1, 0}, normal));
  plane.vertices_.push_back(Vertex({0, 1, 0}, normal));

  plane.elements_ = {1, 3, 0, 1, 2, 3};
  plane.elements_.reserve(6);
  std::for_each(plane.elements_.begin(), plane.elements_.end(),
                [idxOffset](auto& i) { i += *idxOffset; });
  *idxOffset += 4;
  return plane;
}

}  // namespace uinta
