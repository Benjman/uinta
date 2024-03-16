#include "uinta/primitive.h"

#include <algorithm>
#include <utility>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "uinta/vao.h"

namespace uinta {

inline void transformVertex(Vertex*, glm::vec3, f32, glm::vec3) noexcept;

Primitive::Primitive(std::span<Primitive> primitives) noexcept {
  std::for_each(primitives.begin(), primitives.end(), [&](auto& primitive) {
    vertices_.insert(vertices_.end(), primitive.vertices_.begin(),
                     primitive.vertices_.end());
    indices_.insert(indices_.end(), primitive.indices_.begin(),
                    primitive.indices_.end());
  });
}

Primitive::Primitive(Primitive&& other) noexcept
    : vertices_(std::move(other.vertices_)),
      indices_(std::move(other.indices_)) {
  other.vertices_ = {};
  other.indices_ = {};
}

Primitive& Primitive::operator=(Primitive&& other) noexcept {
  if (this != &other) {
    vertices_ = std::move(other.vertices_);
    indices_ = std::move(other.indices_);
  }
  return *this;
}

Primitive Primitive::Cube(size_t idxOffset) noexcept {
  auto front = Plane(idxOffset);
  std::for_each(front.vertices_.begin(), front.vertices_.end(),
                [](auto& vertex) {
                  transformVertex(&vertex, {0.5, -0.5, 0.5}, 180, {0, 1, 0});
                });

  auto back = Plane(idxOffset += 4);
  std::for_each(back.vertices_.begin(), back.vertices_.end(), [](auto& vertex) {
    transformVertex(&vertex, {-0.5, -0.5, -0.5}, 0, {0, 1, 0});
  });

  auto left = Plane(idxOffset += 4);
  std::for_each(left.vertices_.begin(), left.vertices_.end(), [](auto& vertex) {
    transformVertex(&vertex, {-0.5, -0.5, 0.5}, 90, {0, 1, 0});
  });

  auto right = Plane(idxOffset += 4);
  std::for_each(right.vertices_.begin(), right.vertices_.end(),
                [](auto& vertex) {
                  transformVertex(&vertex, {0.5, -0.5, -0.5}, 270, {0, 1, 0});
                });

  auto top = Plane(idxOffset += 4);
  std::for_each(top.vertices_.begin(), top.vertices_.end(), [](auto& vertex) {
    transformVertex(&vertex, {-0.5, 0.5, -0.5}, 90, {1, 0, 0});
  });

  auto bottom = Plane(idxOffset += 4);
  std::for_each(bottom.vertices_.begin(), bottom.vertices_.end(),
                [](auto& vertex) {
                  transformVertex(&vertex, {-0.5, -0.5, 0.5}, 270, {1, 0, 0});
                });

  return Primitive(
      std::vector<Primitive>() = {front, back, left, right, top, bottom});
}

Primitive Primitive::Plane(size_t idxOffset) noexcept {
  constexpr glm::vec3 normal(0, 0, 1);

  Primitive plane(4, 6);
  plane.vertices_.push_back(Vertex({0, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 0, 0}, normal));
  plane.vertices_.push_back(Vertex({1, 1, 0}, normal));
  plane.vertices_.push_back(Vertex({0, 1, 0}, normal));

  plane.indices_ = {0, 3, 1, 3, 2, 1};
  std::for_each(plane.indices_.begin(), plane.indices_.end(),
                [idxOffset](auto& i) { i += idxOffset; });

  return plane;
}

inline void transformVertex(Vertex* vertex, glm::vec3 translate, f32 rotAngle,
                            glm::vec3 rotAxis) noexcept {
  auto transform = glm::translate(glm::mat4(1), translate);
  transform = glm::rotate(transform, glm::radians(rotAngle), rotAxis);
  vertex->position = transform * glm::vec4(vertex->position, 1);
  vertex->normal = glm::rotate(glm::mat4(1), glm::radians(rotAngle), rotAxis) *
                   glm::vec4(vertex->normal, 1);
}

Attribute Primitive::PositionAttribute(const Vbo* vbo) noexcept {
  return Attribute(vbo, 0, 3, GL_FLOAT, GL_FALSE,
                   Vertex::ElementCount * sizeof(GLfloat),
                   reinterpret_cast<void*>(0));
}

Attribute Primitive::NormalAttribute(const Vbo* vbo) noexcept {
  return Attribute(vbo, 1, 3, GL_FLOAT, GL_FALSE,
                   Vertex::ElementCount * sizeof(GLfloat),
                   reinterpret_cast<void*>(3 * sizeof(GLfloat)));
}

Attribute Primitive::ColorAttribute(const Vbo* vbo) noexcept {
  return Attribute(vbo, 2, 3, GL_FLOAT, GL_FALSE,
                   Vertex::ElementCount * sizeof(GLfloat),
                   reinterpret_cast<void*>(6 * sizeof(GLfloat)));
}

Attribute Primitive::UVAttribute(const Vbo* vbo) noexcept {
  return Attribute(vbo, 3, 2, GL_FLOAT, GL_FALSE,
                   Vertex::ElementCount * sizeof(GLfloat),
                   reinterpret_cast<void*>(9 * sizeof(GLfloat)));
}

}  // namespace uinta
