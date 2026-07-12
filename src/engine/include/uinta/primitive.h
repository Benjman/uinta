#ifndef SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <span>
#include <vector>

#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

class Vao;
class Mesh;

class PrimitiveShape {
 public:
  static PrimitiveShape Cube() noexcept;
  static PrimitiveShape Plane() noexcept;
  static PrimitiveShape Sphere(float radius = 1.0f, int segments = 32) noexcept;
  static PrimitiveShape Cylinder(float radius = 0.5f, float height = 1.0f, int segments = 32) noexcept;
  static PrimitiveShape Line(glm::vec3 from, glm::vec3 to) noexcept;
  static PrimitiveShape LineStrip(std::span<const glm::vec3> points) noexcept;

  PrimitiveShape& color(glm::vec3) noexcept;
  PrimitiveShape& transform(glm::mat4) noexcept;
  PrimitiveShape& wireframe(bool) noexcept;

  [[nodiscard]] Mesh build() const noexcept;
  [[nodiscard]] GLenum glPrimitive() const noexcept;
  [[nodiscard]] glm::mat4 transform() const noexcept;
  [[nodiscard]] bool wireframe() const noexcept;

 private:
  enum class Type : u8 { Cube, Plane, Sphere, Cylinder, Line, LineStrip };

  Type type_ = Type::Cube;
  glm::vec3 color_ = {1, 1, 1};
  glm::mat4 transform_ = glm::mat4(1);
  bool wireframe_ = false;

  float sphereRadius_ = 1.0f;
  int sphereSegments_ = 32;
  float cylRadius_ = 0.5f;
  float cylHeight_ = 1.0f;
  int cylSegments_ = 32;

  std::vector<glm::vec3> points_;
};

class PrimitiveShader : public Shader {
 public:
  explicit PrimitiveShader(const OpenGLApi* gl) noexcept;
  void linkAttributes(const Vao* vao) const noexcept override;

  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv view = {"uView", this};
  UniformMatrix4fv projection = {"uProjection", this};
};

class PrimitiveRenderer {
 public:
  explicit PrimitiveRenderer(const OpenGLApi* gl) noexcept;

  void view(const glm::mat4& v) noexcept;
  void projection(const glm::mat4& p) noexcept;

  void draw(const PrimitiveShape& shape) noexcept;
  void draw(std::span<const PrimitiveShape> shapes) noexcept;

 private:
  void uploadAndDraw(const Mesh&, GLenum primitive, const glm::mat4& model, bool wireframe) noexcept;

  const OpenGLApi* gl_;
  PrimitiveShader shader_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_PRIMITIVE_H_
