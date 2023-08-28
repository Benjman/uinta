#ifndef UINTA_GL_FWD_HPP
#define UINTA_GL_FWD_HPP

#include <uinta/types.h>

#include <uinta/error.hpp>

namespace uinta {

struct Fbo;
struct Vao;
struct Vbo;
struct VertexAttrib;

void bindVao(const Vao& vao);
void destroyVao(Vao& vao);
void disableVertexAttribs(Vao& vao);
uinta_error_code enableVertexAttribs(Vao& vao);
uinta_error_code indexBuffer(Vao& vao, const u32* const data, u32 size, u32 offset = 0);
uinta_error_code initVao(Vao& vao);
uinta_error_code initVertexAttribs(Vao& vao);
void unbindVao(const Vao& unused);

void bindVbo(const Vbo& vbo);
void destroyVbo(Vbo& vbo);
uinta_error_code initVbo(Vbo& vbo);
uinta_error_code resizeVbo(Vbo& vbo, u32 size);
void unbindVbo(const Vbo& vao);
uinta_error_code uploadVbo(Vbo& vbo, const void* const data, u32 size, u32 offset = 0);
uinta_error_code uploadVbo(Vbo& vbo, const void* const data, u32 size, u32 offset, bool& resized);

}  // namespace uinta

#endif  // UINTA_GL_FWD_HPP
