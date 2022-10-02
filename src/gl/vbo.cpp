#include <uinta/gl/errors.hpp>
#include <uinta/gl/type_utils.hpp>
#include <uinta/gl/vbo.hpp>

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

void uinta::bind(const Vbo& vbo) {
  glBindBuffer(vbo.target, vbo.id);
  UINTA_glGetError(glBindBuffer);
}

void uinta::destroy(Vbo& vbo) {
  if (vbo.id == GL_ZERO) {
    return;
  }
  glDeleteBuffers(1, &vbo.id);
  UINTA_glGetError(glDeleteBuffers);
}

void uinta::initVbo(Vbo& vbo) {
  glGenBuffers(1, &vbo.id);
  UINTA_glGetError(glGenBuffers);
  SPDLOG_DEBUG("Initialized {} {}.", getGlEnumName(vbo.target), vbo.id);
  bind(vbo);
}

void uinta::resize(Vbo& vbo, GLsizeiptr size) {
  if (vbo.id == GL_ZERO) {
    initVbo(vbo);
  }
  if (vbo.size == 0) {
    bind(vbo);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError(glBufferData);
    SPDLOG_DEBUG("Allocated {} bytes for {} {}.", size, getGlEnumName(vbo.target), vbo.id);
  } else {
    GLuint newId;
    glGenBuffers(1, &newId);
    UINTA_glGetError(glGenBuffers);
    glBindBuffer(vbo.target, newId);
    UINTA_glGetError(glBindBuffer);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError(glBufferData);
    SPDLOG_DEBUG("Resized {} from {} to {} bytes.", getGlEnumName(vbo.target), vbo.size, size);
    if (vbo.size) {
      glBindBuffer(GL_COPY_WRITE_BUFFER, newId);
      UINTA_glGetError(glBindBuffer);
      glBindBuffer(GL_COPY_READ_BUFFER, vbo.id);
      UINTA_glGetError(glBindBuffer);
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vbo.size);
      UINTA_glGetError(glCopyBufferSubData);
      SPDLOG_DEBUG("Copied {} bytes for {} from {} to {}.", vbo.size, getGlEnumName(vbo.target), vbo.id, newId);
    }
    glDeleteBuffers(1, &vbo.id);
    UINTA_glGetError(glDeleteBuffers);
    vbo.id = newId;
  }
  vbo.max = size;
}

void uinta::unbind(const Vbo& vao) {
  glBindBuffer(vao.target, 0);
  UINTA_glGetError(glBindBuffer);
}

bool uinta::upload(Vbo& vbo, const void* const data, GLsizeiptr size, GLsizeiptr offset) {
  bool resized = false;
  if (vbo.id == GL_ZERO) {
    initVbo(vbo);
  }
  if (vbo.max < offset + size) {
    resize(vbo, offset + size);
    resized = true;
  }
  bind(vbo);
  glBufferSubData(vbo.target, offset, size, data);
  UINTA_glGetError(glBufferSubData);
  if (offset + size > vbo.size) {
    vbo.size = offset + size;
  }
  return resized;
}
