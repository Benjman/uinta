#ifndef SRC_ENGINE_INCLUDE_UINTA_VAO_H_
#define SRC_ENGINE_INCLUDE_UINTA_VAO_H_

#include <memory>
#include <span>

#include "uinta/gl.h"
#include "uinta/types.h"
#include "uinta/vbo.h"

namespace uinta {

struct Attribute {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  size_t offset;
};

class Vao {
  friend class VaoGuard;

 public:
  explicit Vao(const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : gl_(gl) {
    gl_->genVertexArrays(1, &id_);
  }

  ~Vao() noexcept {
    gl_->deleteVertexArrays(1, &id_);
    gl_ = nullptr;
    id_ = 0;
  }

  Vao(const Vao&) noexcept = delete;
  Vao& operator=(const Vao&) noexcept = delete;
  Vao(Vao&&) noexcept = delete;
  Vao& operator=(Vao&&) noexcept = delete;

  void linkAttribute(const Attribute& attribute) const noexcept {
    gl_->vertexAttribPointer(attribute.index, attribute.size, attribute.type,
                             attribute.normalized, attribute.stride,
                             reinterpret_cast<void*>(attribute.offset));
    gl_->enableVertexAttribArray(attribute.index);
  }

  BufferSegment ebo(const std::span<u32> buffer,
                    const BufferSegment& offset) noexcept {
    return ebo(buffer, offset.offset + offset.size);
  }

  BufferSegment ebo(const std::span<u32> buffer,
                    GLsizeiptr offset = 0) noexcept {
    const auto size = buffer.size() * sizeof(u32);
    if (!ebo_) {
      ebo_ = std::make_unique<Vbo>(GL_ELEMENT_ARRAY_BUFFER, size, gl_);
      gl_->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_->id());
    }
    return ebo_->bufferData(buffer.data(), size, GL_STATIC_DRAW, offset);
  }

  Vbo* ebo() noexcept { return ebo_.get(); }

  GLuint id() const noexcept { return id_; }

 private:
  GLuint id_ = 0;
  std::unique_ptr<Vbo> ebo_;
  const OpenGLApi* gl_;
};

class VaoGuard {
 public:
  explicit VaoGuard(const Vao* vao, bool isActive = true) noexcept
      : id_(vao->id_), gl_(vao->gl_) {
    if (isActive) activate();
  }

  ~VaoGuard() noexcept {
    if (isActive_) gl_->bindVertexArray(0);
  }

  VaoGuard(const VaoGuard&) noexcept = delete;
  VaoGuard& operator=(const VaoGuard&) noexcept = delete;
  VaoGuard(VaoGuard&&) noexcept = delete;
  VaoGuard& operator=(VaoGuard&&) noexcept = delete;

 private:
  const GLuint id_;
  bool isActive_ = false;
  const OpenGLApi* gl_;

  void activate() noexcept {
    gl_->bindVertexArray(id_);
    isActive_ = true;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VAO_H_
