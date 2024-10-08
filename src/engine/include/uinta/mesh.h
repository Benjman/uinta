#ifndef SRC_ENGINE_INCLUDE_UINTA_MESH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MESH_H_

#include <span>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/lib/absl/status.h"
#include "uinta/types.h"
#include "uinta/utils/generator.h"

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

  glm::vec3 position = {};
  glm::vec3 normal = {};
  glm::vec3 color = {};
  glm::vec2 uv = {};
};

class Mesh {
 public:
  static Mesh Cube(size_t* idxOffset = nullptr,
                   const glm::mat4& transform = glm::mat4(1)) noexcept;
  static Mesh Plane(size_t* idxOffset = nullptr,
                    const glm::mat4& transform = glm::mat4(1)) noexcept;

  Mesh() noexcept = default;
  explicit Mesh(const std::span<Mesh> collection) noexcept;
  explicit Mesh(const std::span<Vertex> collection) noexcept;

  Mesh(const std::span<Vertex> vertices,
       const std::span<u32> elements) noexcept {
    vertices_.assign(vertices.begin(), vertices.end());
    elements_.assign(elements.begin(), elements.end());
  }

  Mesh(const Mesh& other) noexcept;
  Mesh(const Mesh&& other) noexcept;

  Mesh& operator=(const Mesh& other) noexcept;
  Mesh& operator=(const Mesh&& other) noexcept;

  void addVertex(const Vertex& vertex) noexcept {
    vertices_.emplace_back(vertex);
  }

  const std::vector<Vertex>& vertices() const noexcept { return vertices_; }

  std::span<Vertex> vertices() noexcept { return vertices_; }

  const std::vector<u32> elements() const noexcept { return elements_; }

  std::span<u32> elements() noexcept { return elements_; }

  void recalculateNormals(size_t idxOffset = 0) noexcept;

  void color(const glm::vec3& color) noexcept;

  void scale(glm::vec3 scale, glm::mat4 transform = glm::mat4(1)) noexcept;

  void rotate(f32 angle, glm::vec3 axis,
              glm::mat4 transform = glm::mat4(1)) noexcept;

  void translate(glm::vec3 translation,
                 glm::mat4 transform = glm::mat4(1)) noexcept;

  struct Environment {
    static Mesh Tree(size_t* idxOffset,
                     glm::mat4 transform = glm::mat4(1)) noexcept;
  };

 private:
  std::vector<Vertex> vertices_;
  std::vector<u32> elements_;
};

using MeshGenerator = Generator<StatusOr<Mesh>>;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MESH_H_
