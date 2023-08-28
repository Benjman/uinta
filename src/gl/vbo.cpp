#include <uinta/error.hpp>
#include <uinta/gl/utils/errors.hpp>
#include <uinta/gl/utils/type_utils.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/logging.hpp>
#include <uinta/utils/formatters.hpp>

uinta::Vbo::Vbo(GLenum target, GLenum usage) : Vbo(GL_ZERO, target, usage, 0) {
}

uinta::Vbo::Vbo(GLuint id, GLenum target, GLenum usage, GLsizeiptr size)
    : id(id), target(target), usage(usage), size(size), max(size) {
}

uinta::Vbo::Vbo(const Vbo& other) {
  *this = other;
}

uinta::Vbo& uinta::Vbo::operator=(const Vbo& rhs) {
  id = rhs.id;
  target = rhs.target;
  usage = rhs.usage;
  size = rhs.size;
  max = rhs.max;
  return *this;
}

void uinta::bindVbo(const Vbo& vbo) {
  glBindBuffer(vbo.target, vbo.id);
  UINTA_glGetError(glBindBuffer);
}

void uinta::destroyVbo(Vbo& vbo) {
  if (vbo.id == GL_ZERO) return;
  glDeleteBuffers(1, &vbo.id);
  UINTA_glGetError(glDeleteBuffers);
}

uinta::uinta_error_code uinta::initVbo(Vbo& vbo) {
  glGenBuffers(1, &vbo.id);
  UINTA_glGetError(glGenBuffers);
  SPDLOG_DEBUG("Initialized {} {}.", getGlEnumName(vbo.target), vbo.id);
  if (auto error = uploadVbo(vbo, nullptr, 0); error) return error;
  return SUCCESS_EC;
}

uinta::uinta_error_code uinta::resizeVbo(Vbo& vbo, uint size) {
  if (vbo.id == GL_ZERO)
    if (auto error = initVbo(vbo); error) return error;
  if (vbo.size == 0) {
    bindVbo(vbo);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError(glBufferData);
    SPDLOG_DEBUG("Allocated {} for {} {}.", formatMemory(size), getGlEnumName(vbo.target), vbo.id);
  } else {
    GLuint newId;
    glGenBuffers(1, &newId);
    UINTA_glGetError(glGenBuffers);
    glBindBuffer(vbo.target, newId);
    UINTA_glGetError(glBindBuffer);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError(glBufferData);
    SPDLOG_DEBUG("Resized {} from {} to {}.", getGlEnumName(vbo.target), formatMemory(vbo.size), formatMemory(size));
    if (vbo.size) {
      glBindBuffer(GL_COPY_WRITE_BUFFER, newId);
      UINTA_glGetError(glBindBuffer);
      glBindBuffer(GL_COPY_READ_BUFFER, vbo.id);
      UINTA_glGetError(glBindBuffer);
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vbo.size);
      UINTA_glGetError(glCopyBufferSubData);
      SPDLOG_DEBUG("Copied {} for {} from {} to {}.", formatMemory(vbo.size), getGlEnumName(vbo.target), vbo.id, newId);
    }
    glDeleteBuffers(1, &vbo.id);
    UINTA_glGetError(glDeleteBuffers);
    vbo.id = newId;
  }
  vbo.max = size;
  return SUCCESS_EC;
}

void uinta::unbindVbo(const Vbo& vao) {
  glBindBuffer(vao.target, 0);
  UINTA_glGetError(glBindBuffer);
}

uinta::uinta_error_code uinta::uploadVbo(Vbo& vbo, const void* const data, uint size, uint offset) {
  bool resized;
  return uploadVbo(vbo, data, size, offset, resized);
}

uinta::uinta_error_code uinta::uploadVbo(Vbo& vbo, const void* const data, uint size, uint offset, bool& resized) {
  resized = false;
  if (vbo.id == GL_ZERO)
    if (auto error = initVbo(vbo); error) return error;
  if (vbo.max < offset + size) {
    if (auto error = resizeVbo(vbo, offset + size); error) return error;
    resized = true;
  }
  bindVbo(vbo);
  glBufferSubData(vbo.target, offset, size, data);
  UINTA_glGetError(glBufferSubData);
  if (offset + size > vbo.size) {
    vbo.size = offset + size;
  }
  return SUCCESS_EC;
}
