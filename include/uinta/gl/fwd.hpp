#ifndef UINTA_GL_FWD_HPP
#define UINTA_GL_FWD_HPP

#include <uinta/gl/api.h>

namespace uinta {

struct Fbo;
struct Vao;
struct Vbo;
struct VertexAttrib;

void bindVao(const Vao& vao);
void destroyVao(Vao& vao);
void disableVertexAttribs(Vao& vao);
void enableVertexAttribs(Vao& vao);
void indexBuffer(Vao& vao, const GLuint* const data, GLuint size, GLuint offset = 0);
void initVao(Vao& vao);
void initVertexAttribs(Vao& vao);
void unbindVao(const Vao& unused);

void bindVbo(const Vbo& vbo);
void destroyVbo(Vbo& vbo);
void initVbo(Vbo& vbo);
void resizeVbo(Vbo& vbo, GLuint size);
void unbindVbo(const Vbo& vao);
bool uploadVbo(Vbo& vbo, const void* const data, GLuint size, GLuint offset = 0);

}  // namespace uinta

#endif  // UINTA_GL_FWD_HPP
