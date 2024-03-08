#include "uinta/shader.h"

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

Shader::Shader(const std::unordered_map<GLenum, std::string> stagePaths,
               const OpenGLApi* gl, std::shared_ptr<FileSystem> fs) noexcept
    : gl_(gl) {
  auto id = gl_->createProgram();
  LOG(INFO) << "Creating shader: " << id;
  for (auto& pair : stagePaths) {
    LOG(INFO) << "Compiling shader: " << pair.second;
    auto file = File(pair.second, fs);
    if (!file.status().ok()) {
      LOG(FATAL) << file.status().message();
      return;
    }
    auto src = file.readAll();
    const GLchar* data = src.data();
    auto len = static_cast<GLint>(src.size());
    auto shaderId = gl_->createShader(pair.first);
    gl_->shaderSource(shaderId, 1, &data, &len);
    gl_->compileShader(shaderId);
    GLint isCompiled = 0;
    gl_->getShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
      constexpr size_t kInfoLen = 1024;
      GLchar info[kInfoLen];
      GLsizei logSize;
      gl_->getShaderInfoLog(shaderId, kInfoLen, &logSize, info);
      gl_->deleteShader(shaderId);
      LOG(FATAL) << StrFormat("%s: %s", pair.second,
                              std::string(info, logSize));
      return;
    }
    gl_->attachShader(id, shaderId);
    gl_->deleteShader(shaderId);
  }

  gl_->linkProgram(id);
  GLint success;
  gl_->getProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    constexpr size_t kInfoLen = 1024;
    GLchar info[kInfoLen];
    GLsizei logSize;
    gl_->getProgramInfoLog(id, kInfoLen, &logSize, info);
    LOG(FATAL) << std::string(info, logSize);
    return;
  }
  id_ = id;
  LOG(INFO) << "Created shader: " << id_;
}

Shader::~Shader() noexcept {
  gl_->deleteProgram(id_);
  id_ = 0;
}

}  // namespace uinta
