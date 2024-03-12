#ifndef SRC_ENGINE_INCLUDE_UINTA_VBO_H_
#define SRC_ENGINE_INCLUDE_UINTA_VBO_H_

#include "uinta/gl.h"
#include "uinta/types.h"

namespace uinta {

class Vbo;

class VboGuard {
 public:
  explicit VboGuard(const Vbo*) noexcept;
  ~VboGuard() noexcept;
  VboGuard(const VboGuard&) noexcept = delete;
  VboGuard& operator=(const VboGuard&) noexcept = delete;
  VboGuard(VboGuard&&) noexcept = delete;
  VboGuard& operator=(VboGuard&&) noexcept = delete;

 private:
  const GLenum target_;
  const OpenGLApi* gl_;
};

class Vbo {
  friend class Vao;
  friend class VboGuard;

 public:
  Vbo(GLenum target, GLintptr initialSize = 0,
      const OpenGLApi* gl = OpenGLApiImpl::GetInstance()) noexcept
      : target_(target), gl_(gl) {
    gl_->genBuffers(1, &id_);
    if (initialSize) resize(initialSize);
  }

  ~Vbo() noexcept {
    gl_->deleteBuffers(1, &id_);
    id_ = 0;
    target_ = GL_NONE;
  }

  Vbo(const Vbo&) noexcept = delete;
  Vbo& operator=(const Vbo&) noexcept = delete;

  Vbo(Vbo&& other) noexcept
      : id_(other.id_),
        target_(other.target_),
        size_(other.size_),
        gl_(other.gl_) {
    other.id_ = 0;
    other.target_ = 0;
    other.size_ = 0;
    other.gl_ = nullptr;
  }

  Vbo& operator=(Vbo&& other) noexcept {
    if (this != &other) {
      gl_->deleteBuffers(1, &id_);

      id_ = other.id_;
      other.id_ = 0;

      target_ = other.target_;
      other.target_ = 0;

      size_ = other.size_;
      other.size_ = 0;

      gl_ = other.gl_;
      other.gl_ = nullptr;
    }
    return *this;
  }

  template <typename T>
  void bufferData(const T* buffer, u32 count, GLenum usage,
                  GLintptr offset = 0) noexcept {
    if (count + offset > size_) resize((offset + count) * sizeof(T), usage);
    VboGuard guard(this);
    gl_->bufferSubData(target_, offset, count * sizeof(T), buffer);
    size_ = count * sizeof(T);
  }

  GLuint id() const noexcept { return id_; }

  GLenum target() const noexcept { return target_; }

  GLsizeiptr size() const noexcept { return size_; }

 private:
  GLuint id_ = 0;
  GLenum target_ = GL_NONE;
  GLsizeiptr size_ = 0;
  GLsizeiptr max_ = 0;
  const OpenGLApi* gl_;

  void resize(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW) noexcept {
    if (size_ == 0) {
      VboGuard guard(this);
      gl_->bufferData(target_, size, nullptr, usage);
      size_ = size;
    } else {
      GLuint id;
      gl_->genBuffers(1, &id);
      gl_->bindBuffer(target_, id);
      gl_->bufferData(target_, size, nullptr, usage);
      gl_->bindBuffer(GL_COPY_WRITE_BUFFER, id);
      gl_->bindBuffer(GL_COPY_READ_BUFFER, id_);
      gl_->copyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                             size_);
      gl_->deleteBuffers(1, &id_);
      gl_->bindBuffer(GL_COPY_WRITE_BUFFER, 0);
      gl_->bindBuffer(GL_COPY_READ_BUFFER, 0);
      gl_->bindBuffer(target_, 0);
      id_ = id;
    }
    max_ = size;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VBO_H_
