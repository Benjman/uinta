#ifndef SRC_ENGINE_INCLUDE_UINTA_MESH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MESH_H_

#include <span>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/status.h"
#include "uinta/types.h"
#include "uinta/utils/generator.h"

namespace uinta {

struct Vertex {
  using position_type = glm::vec3;
  using normal_type = glm::vec3;
  using color_type = glm::vec3;
  using uv_type = glm::vec2;

  static constexpr size_t ElementCount = 11;

  explicit Vertex(position_type position = {}, normal_type normal = {},
                  color_type color = {1, 1, 1}, uv_type uv = {0, 0}) noexcept
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
      uv = other.uv;
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

  position_type position = {};
  normal_type normal = {};
  color_type color = {};
  uv_type uv = {};
};

class Mesh {
 public:
  using MeshGenerator = Generator<StatusOr<Mesh>>;

  static Mesh Cube(idx_t* idxOffset = nullptr,
                   const glm::mat4& transform = glm::mat4(1)) noexcept;
  static Mesh Plane(idx_t* idxOffset = nullptr,
                    const glm::mat4& transform = glm::mat4(1)) noexcept;

  Mesh() noexcept = default;
  explicit Mesh(const std::span<const Mesh>) noexcept;
  explicit Mesh(const std::span<const Vertex>) noexcept;

  Mesh(const std::span<const Vertex> vertices,
       const std::span<const idx_t> elements) noexcept {
    vertices_.assign(vertices.begin(), vertices.end());
    elements_.assign(elements.begin(), elements.end());
  }

  Mesh(const Mesh&) noexcept;
  Mesh(const Mesh&&) noexcept;

  Mesh& operator=(const Mesh&) noexcept;
  Mesh& operator=(const Mesh&&) noexcept;

  void merge(const Mesh&) noexcept;
  void merge(const std::span<const Mesh>) noexcept;

  void addVertex(const Vertex& vertex) noexcept {
    vertices_.emplace_back(vertex);
  }

  const std::vector<Vertex>& vertices() const noexcept { return vertices_; }

  std::span<Vertex> vertices() noexcept { return vertices_; }

  std::span<const Vertex> verticesc() const noexcept { return vertices_; }

  const std::vector<idx_t>& elements() const noexcept { return elements_; }

  std::span<idx_t> elements() noexcept { return elements_; }

  std::span<const idx_t> elementsc() const noexcept { return elements_; }

  void reserve(size_t vertices, size_t indices = 0) noexcept {
    vertices_.reserve(vertices);
    elements_.reserve(indices);
  }

  void recalculateNormals(size_t idxOffset = 0) noexcept;

  void color(const Vertex::color_type color) noexcept;

  void scale(glm::vec3 scale, glm::mat4 transform = glm::mat4(1)) noexcept;

  void rotate(f32 angle, glm::vec3 axis,
              glm::mat4 transform = glm::mat4(1)) noexcept;

  void translate(glm::vec3 translation,
                 glm::mat4 transform = glm::mat4(1)) noexcept;

  void transformPositions(glm::mat4 transform) noexcept;

 private:
  std::vector<Vertex> vertices_;
  std::vector<idx_t> elements_;
};

using MeshGenerator = Mesh::MeshGenerator;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MESH_H_
