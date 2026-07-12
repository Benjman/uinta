#include "uinta/primitive.h"

#include <manifold/manifold.h>

#include <span>

#include "uinta/manifold_mesh.h"
#include "uinta/mesh.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

// PrimitiveShape

PrimitiveShape PrimitiveShape::Cube() noexcept {
  PrimitiveShape s;
  s.type_ = Type::Cube;
  return s;
}

PrimitiveShape PrimitiveShape::Plane() noexcept {
  PrimitiveShape s;
  s.type_ = Type::Plane;
  return s;
}

PrimitiveShape PrimitiveShape::Sphere(float radius, int segments) noexcept {
  PrimitiveShape s;
  s.type_ = Type::Sphere;
  s.sphereRadius_ = radius;
  s.sphereSegments_ = segments;
  return s;
}

PrimitiveShape PrimitiveShape::Cylinder(float radius, float height, int segments) noexcept {
  PrimitiveShape s;
  s.type_ = Type::Cylinder;
  s.cylRadius_ = radius;
  s.cylHeight_ = height;
  s.cylSegments_ = segments;
  return s;
}

PrimitiveShape PrimitiveShape::Line(glm::vec3 from, glm::vec3 to) noexcept {
  PrimitiveShape s;
  s.type_ = Type::Line;
  s.points_ = {from, to};
  return s;
}

PrimitiveShape PrimitiveShape::LineStrip(std::span<const glm::vec3> points) noexcept {
  PrimitiveShape s;
  s.type_ = Type::LineStrip;
  s.points_.assign(points.begin(), points.end());
  return s;
}

PrimitiveShape& PrimitiveShape::color(glm::vec3 c) noexcept {
  color_ = c;
  return *this;
}

PrimitiveShape& PrimitiveShape::transform(glm::mat4 t) noexcept {
  transform_ = t;
  return *this;
}

PrimitiveShape& PrimitiveShape::wireframe(bool w) noexcept {
  wireframe_ = w;
  return *this;
}

glm::mat4 PrimitiveShape::transform() const noexcept { return transform_; }

bool PrimitiveShape::wireframe() const noexcept { return wireframe_; }

Mesh PrimitiveShape::build() const noexcept {
  Mesh mesh;
  switch (type_) {
    case Type::Cube:
      mesh = Mesh::Cube();
      break;
    case Type::Plane:
      mesh = Mesh::Plane();
      break;
    case Type::Sphere:
      mesh = ToMesh(manifold::Manifold::Sphere(sphereRadius_, sphereSegments_));
      break;
    case Type::Cylinder:
      mesh = ToMesh(manifold::Manifold::Cylinder(cylHeight_, cylRadius_, -1.0f, cylSegments_));
      break;
    case Type::Line:
    case Type::LineStrip:
      for (const auto& p : points_) {
        mesh.addVertex(Vertex{.position = p, .color = color_});
      }
      return mesh;  // no elements — renderer uses drawArrays
  }
  mesh.color(color_);
  return mesh;
}

GLenum PrimitiveShape::glPrimitive() const noexcept {
  switch (type_) {
    case Type::Line:
      return GL_LINES;
    case Type::LineStrip:
      return GL_LINE_STRIP;
    default:
      return GL_TRIANGLES;
  }
}

// PrimitiveShader

PrimitiveShader::PrimitiveShader(const OpenGLApi* gl) noexcept
    : Shader(
          {
              {GL_VERTEX_SHADER, "shader/primitive.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/primitive.fs.glsl"},
          },
          gl) {
  ShaderGuard guard(this);
  model = glm::mat4(1);
  view = glm::mat4(1);
  projection = glm::mat4(1);
}

void PrimitiveShader::linkAttributes(const Vao* vao) const noexcept {
  constexpr auto stride = static_cast<GLsizei>(Vertex::ElementCount * sizeof(GLfloat));
  vao->linkAttribute({
      .index = 0,
      .size = 3,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = stride,
      .offset = 0 * sizeof(GLfloat),
  });
  vao->linkAttribute({
      .index = 1,
      .size = 3,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = stride,
      .offset = 3 * sizeof(GLfloat),
  });
  vao->linkAttribute({
      .index = 2,
      .size = 3,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = stride,
      .offset = 6 * sizeof(GLfloat),
  });
  vao->linkAttribute({
      .index = 3,
      .size = 2,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = stride,
      .offset = 9 * sizeof(GLfloat),
  });
}

// PrimitiveRenderer

PrimitiveRenderer::PrimitiveRenderer(const OpenGLApi* gl) noexcept : gl_(gl), shader_(gl) {}

void PrimitiveRenderer::view(const glm::mat4& v) noexcept {
  ShaderGuard guard(&shader_);
  shader_.view = v;
}

void PrimitiveRenderer::projection(const glm::mat4& p) noexcept {
  ShaderGuard guard(&shader_);
  shader_.projection = p;
}

void PrimitiveRenderer::draw(const PrimitiveShape& shape) noexcept {
  auto mesh = shape.build();
  uploadAndDraw(mesh, shape.glPrimitive(), shape.transform(), shape.wireframe());
}

void PrimitiveRenderer::draw(std::span<const PrimitiveShape> shapes) noexcept {
  for (const auto& shape : shapes) {
    draw(shape);
  }
}

void PrimitiveRenderer::uploadAndDraw(const Mesh& mesh, GLenum primitive, const glm::mat4& model,
                                      bool wireframe) noexcept {
  if (mesh.vertices().empty()) return;

  Vao vao(gl_);
  Vbo vbo(GL_ARRAY_BUFFER, 0, gl_);

  ShaderGuard sg(&shader_);
  VaoGuard vg(&vao);

  {
    VboGuard vbg(&vbo);
    const auto bytes = static_cast<GLsizeiptr>(mesh.vertices().size() * Vertex::ElementCount * sizeof(GLfloat));
    vbo.bufferData(mesh.vertices().data(), bytes, GL_DYNAMIC_DRAW);
    shader_.linkAttributes(&vao);
  }

  shader_.model = model;

  DepthTestGuard dtg;

  const bool indexed = !mesh.elements().empty();
  if (indexed) {
    auto elems = const_cast<Mesh&>(mesh).elements();
    vao.ebo(elems);
  }

  const auto count = static_cast<GLsizei>(indexed ? mesh.elements().size() : mesh.vertices().size());

  auto draw = [&] {
    if (indexed) {
      gl_->drawElements(primitive, count, GL_UNSIGNED_INT, nullptr);
    } else {
      gl_->drawArrays(primitive, 0, count);
    }
  };

  if (wireframe) {
    PolygonMode pm(GL_LINE, gl_);
    draw();
  } else {
    draw();
  }
}

}  // namespace uinta
