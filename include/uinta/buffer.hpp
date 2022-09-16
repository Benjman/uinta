#ifndef UINTA_BUFFER_HPP
#define UINTA_BUFFER_HPP

#include <uinta/gl.h>

#include <vector>

namespace uinta {

struct VertexAttrib {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const void* pointer;

  VertexAttrib() : VertexAttrib(GL_ZERO, 0, GL_TYPE, GL_FALSE, GL_ZERO, GL_ZERO) {
  }

  VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
      : index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer) {
  }

  VertexAttrib(const VertexAttrib& other) {
    *this = other;
  }

  VertexAttrib& operator=(const VertexAttrib& rhs) {
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
  std::vector<VertexAttrib> attribs;
  Vbo indexBuffer = Vbo(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

  Vao(const std::vector<VertexAttrib>& attribs) : attribs(std::vector(attribs)) {
  }

  Vao(const Vao& other) {
    *this = other;
  }

  Vao& operator=(const Vao& rhs) {
    attribs = std::vector<VertexAttrib>(rhs.attribs);
    id = rhs.id;
    indexBuffer = rhs.indexBuffer;
    return *this;
  }
};

inline void bind(const Vao& vao);
inline void bind(const Vbo& vbo);
inline void destroy(Vao& vao);
inline void destroy(Vbo& vbo);
inline void disableVertexAttribs(Vao& vao);
inline void enableVertexAttribs(Vao& vao);
inline void indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size, GLsizeiptr offset = 0);
inline void initVao(Vao& vao);
inline void initVbo(Vbo& vbo);
inline void initVertexAttribs(Vao& vao);
inline void resize(Vbo& vbo, GLsizeiptr size);
inline void unbind(const Vao& unused);
inline void unbind(const Vbo& vao);
inline bool upload(Vbo& vbo, const void* const data, GLsizeiptr size, GLsizeiptr offset = 0);

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
  disableVertexAttribs(vao);
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

inline void disableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO) {
    return;
  }
  for (auto& attrib : vao.attribs) {
    glDisableVertexAttribArray(vao.id);
    UINTA_glGetError("glDisableVertexAttribArray");
  }
}

inline void enableVertexAttribs(Vao& vao) {
  if (vao.id == GL_ZERO) {
    initVao(vao);
  }
  bind(vao);
  for (auto& attrib : vao.attribs) {
    glEnableVertexAttribArray(vao.id);
    UINTA_glGetError("glEnableVertexAttribArray");
  }
}

inline void indexBuffer(Vao& vao, const GLuint* const data, GLsizeiptr size, GLsizeiptr offset) {
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

inline void initVao(Vao& vao) {
  glGenVertexArrays(1, &vao.id);
  UINTA_glGetError("glGenVertexArrays");
  SPDLOG_DEBUG("Initialized VAO {}.", vao.id);
  bind(vao);
}

inline void initVbo(Vbo& vbo) {
  glGenBuffers(1, &vbo.id);
  UINTA_glGetError("glGenBuffers");
  SPDLOG_DEBUG("Initialized {} {}.", getGlEnumName(vbo.target), vbo.id);
  bind(vbo);
}

inline void initVertexAttribs(Vao& vao) {
  for (auto& a : vao.attribs) {
    glVertexAttribPointer(a.index, a.size, a.type, a.normalized, a.stride, a.pointer);
    glEnableVertexAttribArray(a.index);
  }
}

inline void resize(Vbo& vbo, GLsizeiptr size) {
  if (vbo.id == GL_ZERO) {
    initVbo(vbo);
  }
  if (vbo.size == 0) {
    bind(vbo);
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError("glBufferData");
    SPDLOG_DEBUG("Allocated {} bytes for {} {}.", size, getGlEnumName(vbo.target), vbo.id);
  } else {
    GLuint newId;
    glGenBuffers(1, &newId);
    UINTA_glGetError("glGenBuffers");
    glBindBuffer(vbo.target, newId);
    UINTA_glGetError("glBindBuffer");
    glBufferData(vbo.target, size, nullptr, vbo.usage);
    UINTA_glGetError("glBufferData");
    SPDLOG_DEBUG("Resized {} from {} to {} bytes.", getGlEnumName(vbo.target), vbo.size, size);
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
  }
  vbo.max = size;
}

inline void unbind(const Vao& unused) {
  glBindVertexArray(0);
  UINTA_glGetError("glBindVertexArray");
}

inline void unbind(const Vbo& vao) {
  glBindBuffer(vao.target, 0);
  UINTA_glGetError("glBindBuffer");
}

inline bool upload(Vbo& vbo, const void* const data, GLsizeiptr size, GLsizeiptr offset) {
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
  UINTA_glGetError("glBufferSubData");
  if (offset + size > vbo.size) {
    vbo.size = offset + size;
  }
  return resized;
}

}  // namespace uinta

#endif  // UINTA_BUFFER_HPP
