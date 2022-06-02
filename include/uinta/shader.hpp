#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <uinta/gl.h>

#include <string>
#include <vector>

GLuint createShaderProgram(const char** sources,
                           const GLenum* stages,
                           const unsigned int stage_count,
                           const GLint* buffer_lengths,
                           const std::vector<std::string> uniformNames = std::vector<std::string>(),
                           const std::vector<GLuint*> uniformLocations = std::vector<GLuint*>());

#endif // UINTA_SHADER_H
