#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <uinta/gl.h>

#include <string>
#include <vector>

GLuint createShaderProgram(const std::vector<std::string> &sources, const std::vector<GLenum> &stages,
                           const std::vector<std::string> &uniformNames  = std::vector<std::string>(),
                           const std::vector<GLuint *> &uniformLocations = std::vector<GLuint *>());

#endif // UINTA_SHADER_H
