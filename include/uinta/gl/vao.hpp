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

void bindVao(const Vao& vao);
void destroyVao(Vao& vao);
void disableVertexAttribs(Vao& vao);
void enableVertexAttribs(Vao& vao);
void indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size, GLsizeiptr offset = 0);
void initVao(Vao& vao);
void initVertexAttribs(Vao& vao);
void unbindVao(const Vao& unused);

}  // namespace uinta

#endif  // UINTA_VAO_HPP
