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

void bind(const Vbo& vbo);
void destroy(Vbo& vbo);
void initVbo(Vbo& vbo);
void resize(Vbo& vbo, GLsizeiptr size);
void unbind(const Vbo& vao);
bool upload(Vbo& vbo, const void* const data, GLsizeiptr size, GLsizeiptr offset = 0);

}  // namespace uinta

#endif  // UINTA_VBO_HPP
