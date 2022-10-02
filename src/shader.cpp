#include <glad/glad.h>
#include <uinta/gl.h>

#include <cstdio>
#include <uinta/logging.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/macros.hpp>

namespace uinta {

void checkCompileErrors(GLuint shader, GLenum type) {
  if (type == GL_LINK_STATUS) {
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    UINTA_glGetError(glGetProgramiv);
    if (!success) {
      GLchar info[KILOBYTES(1)];
      glGetProgramInfoLog(shader, KILOBYTES(1), 0, info);
      UINTA_glGetError(glGetProgramInfoLog);
      SPDLOG_ERROR(info);
    }
  } else {
    GLint isCompiled = 0;
    glGetShaderiv(type, GL_COMPILE_STATUS, &isCompiled);
    UINTA_glGetError(glGetShaderiv);
    if (!isCompiled) {
      GLchar info[KILOBYTES(1)];
      glGetShaderInfoLog(type, KILOBYTES(1), 0, info);
      UINTA_glGetError(glGetShaderInfoLog);
      SPDLOG_ERROR(info);
    }
  }
}

GLuint createShaderProgram(const std::vector<std::string>& sources, const std::vector<GLenum>& stages,
                           const std::vector<std::string>& uniformNames, const std::vector<GLuint*>& uniformLocations) {
  if (sources.size() != stages.size()) {
    SPDLOG_ERROR("`sources` and `stages` sizes do not match. Aborting shader creation.");
    return GL_ZERO;
  }

  GLuint id = glCreateProgram();
  UINTA_glGetError(glCreateProgram);

  for (auto i = 0; i < stages.size(); i++) {
    const GLuint stageId = glCreateShader(stages.at(i));
    UINTA_glGetError(glCreateShader);
    const GLchar* cstr = sources.at(i).c_str();
    const GLint length = sources.at(i).size();
    glShaderSource(stageId, 1, &cstr, &length);
    UINTA_glGetError(glShaderSource);
    glCompileShader(stageId);
    UINTA_glGetError(glCompileShader);
    checkCompileErrors(id, stageId);
    glAttachShader(id, stageId);
    UINTA_glGetError(glAttachShader);
    glDeleteShader(stageId);
    UINTA_glGetError(glDeleteShader);
  }

  glLinkProgram(id);
  UINTA_glGetError(glLinkProgram);
  checkCompileErrors(id, GL_LINK_STATUS);
  glUseProgram(id);
  UINTA_glGetError(glUseProgram);

  if (uniformNames.size() != uniformLocations.size()) {
    SPDLOG_WARN("`uniformNames` and `uniformLocations` sizes do not match. Ignoring uniform lookup.");
    return id;
  }

  for (auto i = 0; i < uniformNames.size(); i++) {
    GLuint loc = glGetUniformLocation(id, uniformNames.at(i).c_str());
    UINTA_glGetError(glGetUniformLocation);
    if (loc == -1) SPDLOG_WARN("createShaderProgram - Uniform '{}' not found.", uniformNames.at(i));
    *uniformLocations.at(i) = loc;
  }

  return id;
}

}  // namespace uinta
