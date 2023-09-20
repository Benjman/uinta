#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <uinta/gl/api.h>

#include <string>
#include <uinta/fwd.hpp>
#include <vector>

namespace uinta {

uinta_error_code createShaderProgram(GLuint& ref, const std::vector<std::string>& sources, const std::vector<GLenum>& stages,
                                     const std::vector<std::string>& uniformNames = std::vector<std::string>(),
                                     const std::vector<GLuint*>& uniformLocations = std::vector<GLuint*>());

}  // namespace uinta

#endif  // UINTA_SHADER_H
