#ifndef SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_

#include <span>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/types.h"

namespace uinta {

struct Vertex {
  static constexpr size_t ElementCount = 11;

  explicit Vertex(glm::vec3 position = {}, glm::vec3 normal = {},
                  glm::vec3 color = {1, 1, 1}, glm::vec2 uv = {0, 0}) noexcept
      : position(position), normal(normal), color(color), uv(uv) {}

  ~Vertex() noexcept = default;

  Vertex(const Vertex& other) noexcept
      : position(other.position),
        normal(other.normal),
        color(other.color),
        uv(other.uv) {}

  Vertex& operator=(const Vertex& other) noexcept {
    if (this != &other) {
      position = other.position;
      normal = other.normal;
      color = other.color;
      uv = other.color;
    }
    return *this;
  }

  Vertex(Vertex&& other) noexcept
      : position(other.position),
        normal(other.normal),
        color(other.color),
        uv(other.uv) {}

  Vertex& operator=(Vertex&& other) noexcept {
    if (this != &other) {
      position = other.position;
      normal = other.normal;
      color = other.color;
      uv = other.uv;
    }
    return *this;
  }

  glm::vec3 position = {};
  glm::vec3 normal = {};
  glm::vec3 color = {};
  glm::vec2 uv = {};
};

class Vbo;
class Attribute;

class Primitive {
 public:
  static Primitive Cube(size_t idxOffset = 0) noexcept;
  static Primitive Plane(size_t idxOffset = 0) noexcept;

  static Attribute PositionAttribute(const Vbo* vbo) noexcept;
  static Attribute NormalAttribute(const Vbo* vbo) noexcept;
  static Attribute ColorAttribute(const Vbo* vbo) noexcept;
  static Attribute UVAttribute(const Vbo* vbo) noexcept;

  Primitive(u32 numVertices, u32 numIndices) noexcept {
    vertices_.reserve(numVertices);
    indices_.reserve(numIndices);
  }

  Primitive(std::span<Vertex> vertices, std::span<u32> indices) noexcept
      : Primitive(vertices.size(), indices.size()) {
    vertices_.insert(vertices_.begin(), vertices.begin(), vertices.end());
    indices_.insert(indices_.begin(), indices.begin(), indices.end());
  }

  explicit Primitive(std::span<Primitive>) noexcept;

  Primitive(const Primitive& other) noexcept
      : vertices_(other.vertices_), indices_(other.indices_) {}

  Primitive& operator=(const Primitive& other) noexcept {
    if (this != &other) {
      vertices_ = other.vertices_;
      indices_ = other.indices_;
    }
    return *this;
  }

  Primitive(Primitive&& other) noexcept;

  Primitive& operator=(Primitive&& other) noexcept;

  ~Primitive() noexcept = default;

  const std::vector<Vertex>& vertices() const noexcept { return vertices_; }

  std::span<Vertex> vertices() noexcept { return vertices_; }

  const std::vector<u32> indices() const noexcept { return indices_; }

  std::span<u32> indices() noexcept { return indices_; }

  void addData(const std::span<Vertex> vertices,
               const std::span<u32> indices) noexcept {
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
    indices_.insert(indices_.end(), indices.begin(), indices.end());
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<u32> indices_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_
