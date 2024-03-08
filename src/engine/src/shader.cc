#include "uinta/shader.h"

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/gl.h"
#include "uinta/status.h"

namespace uinta {

struct ShaderValidator final {
  ShaderValidator(GLuint id, const OpenGLApi* gl) noexcept {
    GLint success = GL_ZERO;
    gl->getProgramiv(id, GL_LINK_STATUS, &success);
    if (success == GL_ZERO) {
      constexpr size_t kInfoLen = 1024;
      GLchar info[kInfoLen];
      GLsizei logSize;
      gl->getProgramInfoLog(id, kInfoLen, &logSize, info);
      LOG(FATAL) << std::string(info, logSize);
      return;
    }
  }

  operator bool() const noexcept { return status_.ok(); }

  auto message() const noexcept { return status_.message(); }

 private:
  Status status_ = OkStatus();
};

struct ShaderStageValidator final {
  ShaderStageValidator(GLuint shaderId, std::string_view path,
                       const OpenGLApi* gl) noexcept {
    auto isCompiled = GL_ZERO;
    gl->getShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_ZERO) {
      constexpr size_t kInfoLen = 1024;
      GLchar info[kInfoLen];
      GLsizei logSize;
      gl->getShaderInfoLog(shaderId, kInfoLen, &logSize, info);
      gl->deleteShader(shaderId);
      status_ = InternalError(
          absl::StrFormat("%s: %s", path, std::string(info, logSize)));
    }
  }

  operator bool() const noexcept { return status_.ok(); }

  auto message() const noexcept { return status_.message(); }

 private:
  Status status_ = OkStatus();
};

Shader::Shader(const std::unordered_map<GLenum, std::string> stagePaths,
               const OpenGLApi* gl, FileSystem* fs) noexcept
    : id_(gl->createProgram()), gl_(gl) {
  for (auto& pair : stagePaths) {
    File file(pair.second, fs);
    if (!file.status().ok()) {
      LOG(FATAL) << file.status().message();
      return;
    }
    LOG(INFO) << "Compiling shader: " << file.path();
    auto src = file.readAll();
    const GLchar* data = src.data();
    auto len = static_cast<GLint>(src.size());
    auto shaderId = gl_->createShader(pair.first);
    gl_->shaderSource(shaderId, 1, &data, &len);
    gl_->compileShader(shaderId);
    if (auto valid = ShaderStageValidator(shaderId, pair.second, gl_); !valid) {
      id_ = GL_ZERO;
      LOG(FATAL) << valid.message();
      return;
    }
    gl_->attachShader(id_, shaderId);
    gl_->deleteShader(shaderId);
  }

  gl_->linkProgram(id_);
  if (auto valid = ShaderValidator(id_, gl_); valid) {
    LOG(INFO) << "Created shader: " << id_;
  } else {
    id_ = GL_ZERO;
    LOG(FATAL) << valid.message();
  }
}

Shader::~Shader() noexcept {
  gl_->deleteProgram(id_);
  id_ = 0;
}

}  // namespace uinta
