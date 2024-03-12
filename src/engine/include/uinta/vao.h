#ifndef SRC_ENGINE_INCLUDE_UINTA_VAO_H_
#define SRC_ENGINE_INCLUDE_UINTA_VAO_H_

#include <memory>
#include <span>

#include "uinta/gl.h"
#include "uinta/types.h"

namespace uinta {

class Vbo;

struct Attribute {
  const Vbo* vbo;
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const void* pointer;
};

class Vao {
  friend class VaoGuard;

 public:
  explicit Vao(const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  ~Vao() noexcept;

  Vao(const Vao&) noexcept = delete;
  Vao& operator=(const Vao&) noexcept = delete;

  Vao(Vao&&) noexcept;
  Vao& operator=(Vao&&) noexcept;

  void linkAttribute(const Attribute&) const noexcept;

  void indexBuffer(const u32* buffer, u32 count) noexcept;

  void indexBuffer(const std::span<const u32>) noexcept;

  GLuint id() const noexcept { return id_; }

  const Vbo* indexBuffer() const noexcept { return indexBuffer_.get(); }

 private:
  GLuint id_ = 0;
  std::unique_ptr<Vbo> indexBuffer_;
  const OpenGLApi* gl_;
};

class VaoGuard {
 public:
  explicit VaoGuard(const Vao*) noexcept;
  ~VaoGuard() noexcept;
  VaoGuard(const VaoGuard&) noexcept = delete;
  VaoGuard& operator=(const VaoGuard&) noexcept = delete;
  VaoGuard(VaoGuard&&) noexcept = delete;
  VaoGuard& operator=(VaoGuard&&) noexcept = delete;

 private:
  const Vao* vao_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VAO_H_
