#include <uinta/gl/vertex_attrib.hpp>

uinta::VertexAttrib::VertexAttrib() : VertexAttrib(GL_ZERO, 0, GL_TYPE, GL_FALSE, GL_ZERO, (size_t)GL_ZERO) {
}

uinta::VertexAttrib::VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, int32_t offset)
    : VertexAttrib(index, size, type, normalized, stride, (size_t)offset) {
}

uinta::VertexAttrib::VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, uint32_t offset)
    : VertexAttrib(index, size, type, normalized, stride, (size_t)offset) {
}

uinta::VertexAttrib::VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
    : VertexAttrib(index, size, type, normalized, stride, reinterpret_cast<const void*>(offset)) {
}

uinta::VertexAttrib::VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                  const void* pointer)
    : index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer) {
}

uinta::VertexAttrib::VertexAttrib(const VertexAttrib& other) {
  *this = other;
}

uinta::VertexAttrib& uinta::VertexAttrib::operator=(const VertexAttrib& rhs) {
  index = rhs.index;
  size = rhs.size;
  type = rhs.type;
  normalized = rhs.normalized;
  stride = rhs.stride;
  pointer = rhs.pointer;
  return *this;
}
