#ifndef UINTA_BUFFER_HPP
#define UINTA_BUFFER_HPP

#include <uinta/gl.h>

#include <vector>

#include "uinta/gl/type_utils.hpp"

namespace uinta {

struct VertexAttribute {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const void* pointer;

  VertexAttribute() : VertexAttribute(GL_ZERO, 0, GL_TYPE, GL_FALSE, GL_ZERO, GL_ZERO) {
  }

  VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
      : index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer) {
  }

  VertexAttribute(const VertexAttribute& other) {
    *this = other;
  }

  VertexAttribute& operator=(const VertexAttribute& rhs) {
    index = rhs.index;
    size = rhs.size;
    type = rhs.type;
    normalized = rhs.normalized;
    stride = rhs.stride;
    pointer = rhs.pointer;
    return *this;
  }
};

struct Vbo {
  GLuint id = GL_ZERO;
  GLenum target;
  GLenum usage;
  GLsizeiptr size;
  GLsizeiptr max;

  Vbo(GLenum target, GLenum usage) : Vbo(GL_ZERO, target, usage, 0) {
  }

  Vbo(GLuint id, GLenum target, GLenum usage, GLsizeiptr size) : id(id), target(target), usage(usage), size(size), max(size) {
  }

  Vbo(const Vbo& other) {
    *this = other;
  }

  Vbo& operator=(const Vbo& rhs) {
    id = rhs.id;
    target = rhs.target;
    usage = rhs.usage;
    size = rhs.size;
    max = rhs.max;
    return *this;
  }
};

struct Vao {
  GLuint id = GL_ZERO;
  std::vector<VertexAttribute> attributes;
  Vbo indexBuffer = Vbo(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

  Vao(const std::vector<VertexAttribute>& attributes) : attributes(std::vector(attributes)) {
  }

  Vao(const Vao& other) {
    *this = other;
  }

  Vao& operator=(const Vao& rhs) {
    id = rhs.id;
    attributes = std::vector<VertexAttribute>(attributes);
    return *this;
  }
};

inline void bind(const Vao& vao);
inline void bind(const Vbo& vbo);
inline void destroy(Vao& vao);
inline void destroy(Vbo& vbo);
inline void disableVertexAttributes(Vao& vao);
inline void enableVertexAttributes(Vao& vao);
inline void indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size);
inline void initVao(Vao& vao);
inline void initVbo(Vbo& vbo);
inline void resize(Vbo& vbo, GLsizeiptr size);
inline void unbind(const Vao& unused);
inline void unbind(const Vbo& vao);
inline void upload(Vbo& vbo, const GLfloat* const data, GLsizeiptr size);
inline void upload(Vbo& vbo, const GLuint* const data, GLsizeiptr size);

inline void bind(const Vao& vao) {
  glBindVertexArray(vao.id);
  UINTA_glGetError("glBindVertexArray");
}

inline void bind(const Vbo& vbo) {
  glBindBuffer(vbo.target, vbo.id);
  UINTA_glGetError("glBindBuffer");
}

inline void destroy(Vao& vao) {
  if (vao.id == GL_ZERO) {
    return;
  }
  bind(vao);
  disableVertexAttributes(vao);
  glDeleteVertexArrays(1, &vao.id);
  UINTA_glGetError("glDeleteVertexArrays");
  vao = Vao({});
}

inline void destroy(Vbo& vbo) {
  if (vbo.id == GL_ZERO) {
    return;
  }
  glDeleteBuffers(1, &vbo.id);
  UINTA_glGetError("glDeleteBuffers");
}

inline void disableVertexAttributes(Vao& vao) {
  if (vao.id == GL_ZERO) {
    return;
  }
  for (auto& attrib : vao.attributes) {
    glDisableVertexAttribArray(vao.id);
    UINTA_glGetError("glDisableVertexAttribArray");
  }
}

inline void enableVertexAttributes(Vao& vao) {
  if (vao.id == GL_ZERO) {
    initVao(vao);
  }
  bind(vao);
  for (auto& attrib : vao.attributes) {
    glEnableVertexAttribArray(vao.id);
    UINTA_glGetError("glEnableVertexAttribArray");
  }
}

inline void indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size) {
  if (vao.id == GL_ZERO) {
    initVao(vao);
  }
  if (vao.indexBuffer.id == GL_ZERO) {
    initVbo(vao.indexBuffer);
  }
  upload(vao.indexBuffer, data, size);
}

inline void initVao(Vao& vao) {
  glGenVertexArrays(1, &vao.id);
  UINTA_glGetError("glGenVertexArrays");
  SPDLOG_DEBUG("VAO initialized with ID {}.", vao.id);
  enableVertexAttributes(vao);
  bind(vao);
}

inline void initVbo(Vbo& vbo) {
  glGenBuffers(1, &vbo.id);
  UINTA_glGetError("glGenBuffers");
  SPDLOG_DEBUG("{} initialized with ID {}.", getGlEnumName(vbo.target), vbo.id);
  bind(vbo);
}

inline void resize(Vbo& vbo, GLsizeiptr size) {
  if (vbo.id == GL_ZERO) {
    initVbo(vbo);
  }
  if (vbo.size == 0) {
    bind(vbo);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError("glBufferData");
    SPDLOG_DEBUG("Allocated {} bytes for {}.", size, getGlEnumName(vbo.target));
  } else {
    GLuint newId;
    glGenBuffers(1, &newId);
    UINTA_glGetError("glGenBuffers");
    glBindBuffer(vbo.target, newId);
    UINTA_glGetError("glBindBuffer");
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError("glBufferData");
    SPDLOG_DEBUG("Resized {} from {} to {}.", getGlEnumName(vbo.target), vbo.size, size);
    if (vbo.size) {
      glBindBuffer(GL_COPY_WRITE_BUFFER, newId);
      UINTA_glGetError("glBindBuffer");
      glBindBuffer(GL_COPY_READ_BUFFER, vbo.id);
      UINTA_glGetError("glBindBuffer");
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vbo.size);
      UINTA_glGetError("glCopyBufferSubData");
      SPDLOG_DEBUG("Copied {} bytes for {} from {} to {}.", vbo.size, getGlEnumName(vbo.target), vbo.id, newId);
    }
    glDeleteBuffers(1, &vbo.id);
    UINTA_glGetError("glDeleteBuffers");
    vbo.id = newId;
    vbo.max = size;
  }
}

inline void unbind(const Vao& unused) {
  glBindVertexArray(0);
  UINTA_glGetError("glBindVertexArray");
}

inline void unbind(const Vbo& vao) {
  glBindBuffer(vao.target, 0);
  UINTA_glGetError("glBindBuffer");
}

inline void upload(Vbo& vbo, const GLfloat* const data, GLsizeiptr size) {
  if (vbo.id == GL_ZERO) {
    initVbo(vbo);
  }
  if (vbo.max < vbo.size + size) {
    resize(vbo, vbo.size + size);
  }
  bind(vbo);
  glBufferSubData(vbo.target, vbo.size, size, data);
  UINTA_glGetError("glBufferSubData");
  vbo.size += size;
}

inline void upload(Vbo& vbo, const GLuint* const data, GLsizeiptr size) {
  if (vbo.max < vbo.size + size) {
    resize(vbo, vbo.size + size);
  }
  bind(vbo);
  glBufferSubData(vbo.target, vbo.size, size, data);
  UINTA_glGetError("glBufferSubData");
  vbo.size += size;
}

}  // namespace uinta

#endif  // UINTA_BUFFER_HPP
