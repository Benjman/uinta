#ifndef UINTA_VBO_HPP
#define UINTA_VBO_HPP

#include <uinta/gl/api.hpp>

namespace uinta {

struct Vbo {
  GLuint id = GL_ZERO;
  GLenum target;
  GLenum usage;
  GLsizeiptr size;
  GLsizeiptr max;

  Vbo(GLenum target, GLenum usage);
  Vbo(GLuint id, GLenum target, GLenum usage, GLsizeiptr size);
  Vbo(const Vbo& other);
  Vbo& operator=(const Vbo& rhs);
};

}  // namespace uinta

#endif  // UINTA_VBO_HPP
