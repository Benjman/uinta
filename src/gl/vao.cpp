#include <uinta/error.hpp>
#include <uinta/gl/fwd.hpp>
#include <uinta/gl/utils/errors.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vertex_attrib.hpp>
#include <uinta/logging.hpp>

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

void uinta::bindVao(const Vao& vao) {
  glBindVertexArray(vao.id);
  UINTA_glGetError(glBindVertexArray);
}

void uinta::destroyVao(Vao& vao) {
  if (vao.id == GL_ZERO) return;
  bindVao(vao);
  disableVertexAttribs(vao);
  glDeleteVertexArrays(1, &vao.id);
  UINTA_glGetError(glDeleteVertexArrays);
  vao = Vao({});
}

void uinta::disableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO) return;
  bindVao(vao);
  for (const auto& attrib : vao.attribs) {
    glDisableVertexAttribArray(attrib.index);
    UINTA_glGetError(glDisableVertexAttribArray);
  }
}

uinta::uinta_error_code uinta::enableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO)
    if (auto error = initVao(vao); error) return error;
  bindVao(vao);
  for (const auto& attrib : vao.attribs) {
    glEnableVertexAttribArray(attrib.index);
    UINTA_glGetError(glEnableVertexAttribArray);
  }
  return SUCCESS_EC;
}

uinta::uinta_error_code uinta::indexBuffer(Vao& vao, const u32* const data, u32 size, u32 offset) {
  if (vao.id == GL_ZERO)
    if (auto error = initVao(vao); error) return error;
  if (vao.indexBuffer.id == GL_ZERO)
    if (auto error = initVbo(vao.indexBuffer); error) return error;
  bindVao(vao);
  if (uploadVbo(vao.indexBuffer, data, size, offset))
    if (auto error = initVertexAttribs(vao); error) return error;
  return SUCCESS_EC;
}

uinta::uinta_error_code uinta::initVao(Vao& vao) {
  if (!vao.id) {
    glGenVertexArrays(1, &vao.id);
    UINTA_glGetError(glGenVertexArrays);
    SPDLOG_DEBUG("Initialized VAO {}.", vao.id);
    bindVao(vao);
  }
  return SUCCESS_EC;
}

uinta::uinta_error_code uinta::initVertexAttribs(Vao& vao) {
  if (!vao.id)
    if (auto error = initVao(vao); error) return error;
  for (auto& a : vao.attribs) {
    glVertexAttribPointer(a.index, a.size, a.type, a.normalized, a.stride, a.pointer);
    glEnableVertexAttribArray(a.index);
  }
  return SUCCESS_EC;
}

void uinta::unbindVao(const Vao& unused) {
  glBindVertexArray(0);
  UINTA_glGetError(glBindVertexArray);
}
