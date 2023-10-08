#include <uinta/gl/api.h>
#include <uinta/utils/macros.h>

#include <cstdio>
#include <map>
#include <uinta/error.hpp>
#include <uinta/logging.hpp>
#include <uinta/shader.hpp>

namespace uinta {

enum class error : u8 {
  LinkError = 10,
  StageError = 20,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::LinkError), "OpenGL encountered an error attempting to link a shader program!"},
    {static_cast<uinta_error_code_t>(error::StageError), "OpenGL encountered an error attempting to link a shader stage!"},
};

UINTA_ERROR_FRAMEWORK(Shader, errorMessages);

uinta_error_code checkCompileErrors(GLuint shader, GLenum type) {
  if (type == GL_LINK_STATUS) {
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      GLchar info[KILOBYTES(1)];
      glGetProgramInfoLog(shader, KILOBYTES(1), 0, info);
      SPDLOG_ERROR(info);
      return make_error(error::LinkError);
    }
  } else {
    GLint isCompiled = 0;
    glGetShaderiv(type, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
      GLchar info[KILOBYTES(1)];
      glGetShaderInfoLog(type, KILOBYTES(1), 0, info);
      SPDLOG_ERROR(info);
      return make_error(error::StageError);
    }
  }
  return SUCCESS_EC;
}

uinta_error_code createShaderProgram(u32& ref, const std::vector<std::string>& sources, const std::vector<u32>& stages,
                                     const std::vector<std::string>& uniformNames, const std::vector<u32*>& uniformLocations) {
  ref = GL_ZERO;

  if (sources.size() != stages.size()) return make_error(error::StageError);
  if (uniformNames.size() != uniformLocations.size())
    SPDLOG_WARN("Mismatch between number of uniform names (" + std::to_string(uniformNames.size()) +
                ") and number of location pointers (" + std::to_string(uniformLocations.size()) + ").");

  ref = glCreateProgram();

  for (size_t i = 0; i < stages.size(); i++) {
    const GLuint stageId = glCreateShader(stages.at(i));
    const GLchar* cstr = sources.at(i).c_str();
    const GLint length = sources.at(i).size();
    glShaderSource(stageId, 1, &cstr, &length);
    glCompileShader(stageId);
    if (auto error = checkCompileErrors(ref, stageId); error) return error;
    glAttachShader(ref, stageId);
    glDeleteShader(stageId);
  }

  glLinkProgram(ref);
  if (auto error = checkCompileErrors(ref, GL_LINK_STATUS); error) return error;
  glUseProgram(ref);

  for (size_t i = 0; i < uniformNames.size(); i++) {
    auto loc = glGetUniformLocation(ref, uniformNames.at(i).c_str());
    if (loc == -1)
      SPDLOG_WARN("createShaderProgram - Uniform '{}' not found.", uniformNames.at(i));
    else
      SPDLOG_DEBUG("Found uniform `{}`({}) for shader {}.", uniformNames.at(i), loc, ref);
    *uniformLocations.at(i) = loc;
  }

  return SUCCESS_EC;
}

}  // namespace uinta
