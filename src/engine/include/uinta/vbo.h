#ifndef SRC_ENGINE_INCLUDE_UINTA_VBO_H_
#define SRC_ENGINE_INCLUDE_UINTA_VBO_H_

#include <algorithm>

#include "uinta/gl.h"

namespace uinta {

class Vbo;

class VboGuard {
 public:
  explicit VboGuard(const Vbo*, bool isActive = true) noexcept;
  ~VboGuard() noexcept;
  VboGuard(const VboGuard&) noexcept = delete;
  VboGuard& operator=(const VboGuard&) noexcept = delete;
  VboGuard(VboGuard&&) noexcept = delete;
  VboGuard& operator=(VboGuard&&) noexcept = delete;

  void activate() noexcept;

 private:
  const Vbo* vbo_;
  bool isActive_ = false;
};

class Vbo {
  friend class VboGuard;

 public:
  Vbo(GLenum target = GL_ARRAY_BUFFER, GLsizeiptr initialSize = 0,
      const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : target_(target), gl_(gl) {
    gl_->genBuffers(1, &id_);

    if (initialSize) {
      VboGuard guard(this, true);
      resize(initialSize, GL_STATIC_DRAW);
    }
  }

  ~Vbo() noexcept {
    gl_->deleteBuffers(1, &id_);
    gl_ = nullptr;
    id_ = 0;
    target_ = GL_NONE;
    capacity_ = 0;
    size_ = 0;
  }

  Vbo(const Vbo&) noexcept = delete;
  Vbo(const Vbo&&) noexcept = delete;
  Vbo& operator=(Vbo&& other) noexcept = delete;
  Vbo& operator=(const Vbo& other) noexcept = delete;

  template <typename T>
  BufferSegment bufferData(const T* buffer, GLsizeiptr size, GLenum usage,
                           const BufferSegment& offset) noexcept {
    return bufferData(buffer, size, usage, offset.offset + offset.size);
  }

  template <typename T>
  BufferSegment bufferData(const T* buffer, GLsizeiptr size, GLenum usage,
                           GLintptr offset = 0) noexcept {
    if (offset + size > capacity_) resize(offset + size, usage);
    gl_->bufferSubData(target_, offset, size, buffer);
    size_ = std::max(offset + size, size_);
    return {offset, size};
  }

  GLuint id() const noexcept { return id_; }

  GLenum target() const noexcept { return target_; }

  GLsizeiptr capacity() const noexcept { return capacity_; }

  GLsizeiptr size() const noexcept { return size_; }

  void resize(GLsizeiptr size, GLenum usage) noexcept {
    if (size == capacity_) return;
    if (capacity_ == 0) {
      gl_->bufferData(target_, size, nullptr, usage);
    } else {
      GLuint id;
      gl_->genBuffers(1, &id);
      gl_->bindBuffer(target_, id);
      gl_->bufferData(target_, size, nullptr, usage);
      gl_->bindBuffer(GL_COPY_WRITE_BUFFER, id);
      gl_->bindBuffer(GL_COPY_READ_BUFFER, id_);
      gl_->copyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                             capacity_);
      gl_->bindBuffer(GL_COPY_WRITE_BUFFER, 0);
      gl_->bindBuffer(GL_COPY_READ_BUFFER, 0);
      gl_->deleteBuffers(1, &id_);
      id_ = id;
    }
    capacity_ = size;
  }

 private:
  GLuint id_ = 0;
  GLenum target_ = GL_NONE;
  GLsizeiptr capacity_ = 0;
  GLsizeiptr size_ = 0;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VBO_H_
