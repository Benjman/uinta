#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "uinta/file.h"
#include "uinta/gl.h"

namespace uinta {

class Shader {
  friend class ShaderGuard;

 public:
  Shader(const std::unordered_map<GLenum, std::string> stagePaths,
         const OpenGLApi* = OpenGLApiImpl::GetInstance(),
         std::shared_ptr<FileSystem> =
             std::make_shared<FileSystemImpl>()) noexcept;

  ~Shader() noexcept;

  Shader(const Shader&) noexcept = delete;
  Shader& operator=(const Shader&) noexcept = delete;
  Shader(Shader&&) noexcept = delete;
  Shader& operator=(Shader&&) noexcept = delete;

  GLuint id() const noexcept { return id_; }

  const OpenGLApi* gl() const noexcept { return gl_; }

 protected:
  const OpenGLApi* gl_;

 private:
  GLuint id_ = 0;
};

class ShaderGuard {
 public:
  explicit ShaderGuard(const Shader* shader) noexcept : gl_(shader->gl_) {
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
