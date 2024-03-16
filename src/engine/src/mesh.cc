#include "uinta/mesh.h"

#include <algorithm>
#include <array>
#include <utility>
#include <vector>

#include "absl/random/random.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/color.h"
#include "uinta/math/defs.h"
#include "uinta/math/direction.h"

namespace uinta {

Mesh::Mesh(const std::span<const Mesh> collection) noexcept {
  std::for_each(collection.begin(), collection.end(), [this](auto& mesh) {
    vertices_.insert(vertices_.end(), mesh.vertices().begin(),
                     mesh.vertices().end());
    elements_.insert(elements_.end(), mesh.elements().begin(),
                     mesh.elements().end());
  });
}

Mesh::Mesh(const std::span<const Vertex> vertices) noexcept {
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

void Mesh::merge(const Mesh& other) noexcept {
  vertices_.insert(vertices_.end(), other.vertices_.begin(),
                   other.vertices_.end());
  elements_.insert(elements_.end(), other.elements_.begin(),
                   other.elements_.end());
}

void Mesh::merge(const std::span<const Mesh> others) noexcept {
  std::for_each(others.begin(), others.end(), [&](const auto& other) {
    vertices_.insert(vertices_.end(), other.vertices_.begin(),
                     other.vertices_.end());
    elements_.insert(elements_.end(), other.elements_.begin(),
                     other.elements_.end());
  });
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

void Mesh::scale(glm::vec3 scale, glm::mat4 transform) noexcept {
  transformPositions(glm::scale(transform, scale));
}

void Mesh::rotate(f32 angle, glm::vec3 axis, glm::mat4 transform) noexcept {
  transformPositions(glm::rotate(transform, angle, axis));
  recalculateNormals();
}

void Mesh::translate(glm::vec3 translation, glm::mat4 transform) noexcept {
  transformPositions(glm::translate(transform, translation));
}

void Mesh::transformPositions(glm::mat4 transform) noexcept {
  for (auto& vertex : vertices_) {
    vertex.position = glm::vec3(transform * glm::vec4(vertex.position, 1.0));
  }
}

Mesh Mesh::Cube(idx_t* idxOffset, const glm::mat4& transform) noexcept {
  u32 idx = idxOffset ? *idxOffset : 0;
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
    auto& face = faces.at(i) = Plane(&idx);
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

  if (idxOffset) *idxOffset += idx - *idxOffset;
  return Mesh(faces);
}

Mesh Mesh::Plane(idx_t* idxOffset, const glm::mat4& transform) noexcept {
  size_t idx = idxOffset ? *idxOffset : 0;
  constexpr Vertex::normal_type normal(0, 0, 1);

  std::vector<Vertex> vertices;
  vertices.reserve(4);
  vertices.push_back(Vertex({0, 0, 0}, normal));
  vertices.push_back(Vertex({1, 0, 0}, normal));
  vertices.push_back(Vertex({1, 1, 0}, normal));
  vertices.push_back(Vertex({0, 1, 0}, normal));

  std::for_each(vertices.begin(), vertices.end(), [transform](auto& vertex) {
    vertex.position = transform * glm::vec4(vertex.position, 1);
    vertex.normal = glm::normalize(glm::inverseTranspose(glm::mat3(transform)) *
                                   vertex.normal);
  });

  std::vector<idx_t> elements = {1, 3, 0, 1, 2, 3};
  std::for_each(elements.begin(), elements.end(), [idx](auto& i) { i += idx; });
  if (idxOffset) *idxOffset += 4;
  return Mesh(vertices, elements);
}

void Mesh::color(const Vertex::color_type color) noexcept {
  std::for_each(vertices_.begin(), vertices_.end(),
                [&color](auto& vertex) { vertex.color = color; });
}

void displacePositions(Mesh* mesh, const glm::vec3& range) {
  absl::BitGen bitgen;
  absl::uniform_real_distribution<f32> displacement_x(-range.x, range.x);
  absl::uniform_real_distribution<f32> displacement_y(-range.y, range.y);
  absl::uniform_real_distribution<f32> displacement_z(-range.z, range.z);
  std::vector<std::pair<glm::vec3, glm::vec3>> map;
  for (auto& vertex : mesh->vertices()) {
    auto& position = vertex.position;
    if (std::find_if(map.begin(), map.end(), [&position](auto& kvp) {
          return glm::length(kvp.first - position) < epsilon_f;
        }) == map.end()) {
      map.push_back(
          {position, glm::vec3(displacement_x(bitgen), displacement_y(bitgen),
                               displacement_z(bitgen))});
    }
  }
  std::for_each(
      mesh->vertices().begin(), mesh->vertices().end(), [&map](auto& vertex) {
        auto rand = std::find_if(map.begin(), map.end(), [&vertex](auto& kvp) {
          return glm::length(kvp.first - vertex.position) < epsilon_f;
        });
        vertex.position = glm::translate(glm::mat4(1), rand->second) *
                          glm::vec4(vertex.position, 1);
      });
}

void displaceRotation(Mesh* mesh, f32 deg, const glm::vec3& axis) {
  absl::BitGen bitgen;
  absl::uniform_real_distribution<f32> displacement(-deg, deg);
  std::vector<std::pair<glm::vec3, glm::vec3>> map;
  for (auto& vertex : mesh->vertices()) {
    auto& rotation = vertex.position;
    if (std::find_if(map.begin(), map.end(), [&rotation](auto& kvp) {
          return glm::length(kvp.first - rotation) < epsilon_f;
        }) == map.end()) {
      map.push_back(
          {rotation, glm::vec3(displacement(bitgen), displacement(bitgen),
                               displacement(bitgen)) *
                         axis});
    }
  }
  std::for_each(
      mesh->vertices().begin(), mesh->vertices().end(), [&map](auto& vertex) {
        auto rand = std::find_if(map.begin(), map.end(), [&vertex](auto& kvp) {
          return glm::length(kvp.first - vertex.position) < epsilon_f;
        });
        auto rotation = glm::rotate(glm::mat4(1), glm::radians(rand->second.x),
                                    glm::vec3(1, 0, 0));
        rotation = glm::rotate(rotation, glm::radians(rand->second.y), WorldUp);
        rotation = glm::rotate(rotation, glm::radians(rand->second.z),
                               glm::vec3(0, 0, 1));
        vertex.position = rotation * glm::vec4(vertex.position, 1.0);
      });
}

}  // namespace uinta
