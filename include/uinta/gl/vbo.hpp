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

void bindVbo(const Vbo& vbo);
void destroyVbo(Vbo& vbo);
void initVbo(Vbo& vbo);
void resizeVbo(Vbo& vbo, GLsizeiptr size);
void unbindVbo(const Vbo& vao);
bool uploadVbo(Vbo& vbo, const void* const data, GLsizeiptr size, GLsizeiptr offset = 0);

}  // namespace uinta

#endif  // UINTA_VBO_HPP
