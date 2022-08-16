#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <uinta/logging.hpp>
#include <uinta/macros.hpp>
#include <uinta/shader.hpp>

namespace uinta {
void checkCompileErrors(const GLuint shader, const GLenum type) {
  GLint success = 1;
  if (type == GL_LINK_STATUS) {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      GLchar info[KILOBYTES(1)];
      glGetProgramInfoLog(shader, KILOBYTES(1), 0, info);
    }
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLchar info[KILOBYTES(1)];
      glGetShaderInfoLog(shader, KILOBYTES(1), 0, info);
    }
  }
}

GLuint createShaderProgram(const std::vector<std::string>& sources, const std::vector<GLenum>& stages,
                           const std::vector<std::string>& uniformNames, const std::vector<GLuint*>& uniformLocations) {
  GLuint id = glCreateProgram();

  if (sources.size() != stages.size()) {
    SPDLOG_ERROR("`sources` and `stages` sizes do not match. Aborting shader creation.");
    return GL_ZERO;
  }

  for (auto i = 0; i < stages.size(); i++) {
    const GLuint stageId = glCreateShader(stages.at(i));
    const GLchar* cstr = sources.at(i).c_str();
    const GLint length = sources.at(i).size();
    glShaderSource(stageId, 1, &cstr, &length);
    glCompileShader(stageId);
    checkCompileErrors(id, stageId);
    glAttachShader(id, stageId);
    glDeleteShader(stageId);
  }

  glLinkProgram(id);
  checkCompileErrors(id, GL_LINK_STATUS);
  glUseProgram(id);

  if (uniformNames.size() != uniformLocations.size()) {
    SPDLOG_WARN("`uniformNames` and `uniformLocations` sizes do not match. Ignoring uniform lookup.");
    return id;
  }

  for (auto i = 0; i < uniformNames.size(); i++) {
    GLuint loc = glGetUniformLocation(id, uniformNames.at(i).c_str());
    if (loc == -1) SPDLOG_ERROR("createShaderProgram - Uniform '{}' not found.", uniformNames.at(i));
    *uniformLocations.at(i) = loc;
  }

  return id;
}

}  // namespace uinta

void checkCompileErrors(const GLuint shader, const GLenum type) noexcept {}
