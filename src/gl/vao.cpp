#include <uinta/gl/errors.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vertex_attrib.hpp>

uinta::Vao::Vao(const std::vector<VertexAttrib>& attribs) : attribs(attribs) {
}

uinta::Vao::Vao(const Vao& other) {
  *this = other;
}

uinta::Vao& uinta::Vao::operator=(const Vao& rhs) {
  attribs = std::vector<VertexAttrib>(rhs.attribs);
  id = rhs.id;
  indexBuffer = rhs.indexBuffer;
  return *this;
}

void uinta::bind(const Vao& vao) {
  glBindVertexArray(vao.id);
  UINTA_glGetError(glBindVertexArray);
}

void uinta::destroy(Vao& vao) {
  if (vao.id == GL_ZERO) {
    return;
  }
  bind(vao);
  disableVertexAttribs(vao);
  glDeleteVertexArrays(1, &vao.id);
  UINTA_glGetError(glDeleteVertexArrays);
  vao = Vao({});
}

void uinta::disableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO) {
    return;
  }
  for (auto& attrib : vao.attribs) {
    glDisableVertexAttribArray(vao.id);
    UINTA_glGetError(glDisableVertexAttribArray);
  }
}

void uinta::enableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO) {
    initVao(vao);
  }
  bind(vao);
  for (auto& attrib : vao.attribs) {
    glEnableVertexAttribArray(vao.id);
    UINTA_glGetError(glEnableVertexAttribArray);
  }
}

void uinta::indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size, GLsizeiptr offset) {
  if (vao.id == GL_ZERO) {
    initVao(vao);
  }
  if (vao.indexBuffer.id == GL_ZERO) {
    initVbo(vao.indexBuffer);
  }
  bind(vao);
  if (upload(vao.indexBuffer, data, size, offset)) {
    initVertexAttribs(vao);
  }
}

void uinta::initVao(Vao& vao) {
  glGenVertexArrays(1, &vao.id);
  UINTA_glGetError(glGenVertexArrays);
  SPDLOG_DEBUG("Initialized VAO {}.", vao.id);
  bind(vao);
}

void uinta::initVertexAttribs(Vao& vao) {
  if (!vao.id) {
    initVao(vao);
  }
  for (auto& a : vao.attribs) {
    glVertexAttribPointer(a.index, a.size, a.type, a.normalized, a.stride, a.pointer);
    glEnableVertexAttribArray(a.index);
  }
}

void uinta::unbind(const Vao& unused) {
  glBindVertexArray(0);
  UINTA_glGetError(glBindVertexArray);
}
