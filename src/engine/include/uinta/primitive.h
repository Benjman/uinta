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
  static Primitive Cube(size_t* idxOffset) noexcept;
  static Primitive Plane(size_t* idxOffset) noexcept;

  Primitive() noexcept = default;

  explicit Primitive(const std::span<Primitive> collection) noexcept;

  Primitive(const std::span<Vertex> vertices,
            const std::span<u32> elements) noexcept {
    vertices_.assign(vertices.begin(), vertices.end());
    elements_.assign(elements.begin(), elements.end());
  }

  Primitive(const Primitive& other) noexcept;
  Primitive(const Primitive&& other) noexcept;

  Primitive& operator=(const Primitive& other) noexcept;
  Primitive& operator=(const Primitive&& other) noexcept;

  const std::vector<Vertex>& vertices() const noexcept { return vertices_; }

  std::span<Vertex> vertices() noexcept { return vertices_; }

  const std::vector<u32> elements() const noexcept { return elements_; }

  std::span<u32> elements() noexcept { return elements_; }

  void recalculateNormals(size_t idxOffset = 0) noexcept;

 private:
  std::vector<Vertex> vertices_;
  std::vector<u32> elements_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_
