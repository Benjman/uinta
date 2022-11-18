#ifndef UINTA_VAO_HPP
#define UINTA_VAO_HPP

#include <uinta/gl/api.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/gl/vertex_attrib.hpp>
#include <vector>

namespace uinta {

struct Vao {
  GLuint id = GL_ZERO;
  std::vector<VertexAttrib> attribs;
  Vbo indexBuffer{GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW};

  Vao(const std::vector<VertexAttrib>& attribs);
  Vao(const Vao& other);
  Vao& operator=(const Vao& rhs);
};

}  // namespace uinta

#endif  // UINTA_VAO_HPP
