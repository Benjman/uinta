#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADER_H_

#include <string>
#include <unordered_map>

#include "uinta/file.h"
#include "uinta/gl.h"

namespace uinta {

class Vao;

class Shader {
 public:
  Shader(const std::unordered_map<GLenum, std::string> stagePaths,
         const OpenGLApi*, FileSystem* = FileSystemImpl::Instance()) noexcept;

  ~Shader() noexcept;

  Shader(const Shader&) noexcept = delete;
  Shader& operator=(const Shader&) noexcept = delete;
  Shader(Shader&&) noexcept = delete;
  Shader& operator=(Shader&&) noexcept = delete;

  GLuint id() const noexcept { return id_; }

  const OpenGLApi* gl() const noexcept { return gl_; }

  virtual void linkAttributes(const Vao*) const noexcept {}

 private:
  GLuint id_ = 0;
  const OpenGLApi* gl_;
};

class ShaderGuard {
 public:
  explicit ShaderGuard(const Shader* shader) noexcept : gl_(shader->gl()) {
    gl_->useProgram(shader->id());
  }

  ~ShaderGuard() noexcept { gl_->useProgram(0); }

  ShaderGuard(const ShaderGuard&) noexcept = delete;
  ShaderGuard& operator=(const ShaderGuard&) noexcept = delete;
  ShaderGuard(ShaderGuard&&) noexcept = delete;
  ShaderGuard& operator=(ShaderGuard&&) noexcept = delete;

 protected:
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADER_H_
