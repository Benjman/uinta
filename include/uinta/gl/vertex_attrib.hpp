#ifndef UINTA_VERTEX_ATTRIB_HPP
#define UINTA_VERTEX_ATTRIB_HPP

#include <uinta/gl/api.hpp>

namespace uinta {

struct VertexAttrib {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const void* pointer;

  VertexAttrib();
  VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
  VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, int32_t offset);
  VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);
  VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, uint32_t offset);
  VertexAttrib(const VertexAttrib& other);

  VertexAttrib& operator=(const VertexAttrib& rhs);
};

}  // namespace uinta

#endif  // UINTA_VERTEX_ATTRIB_HPP
