#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <gl.h>

GLuint create_shader_program(const char** sources,
                             const GLenum* stages,
                             const unsigned int stage_count,
                             const GLint* buffer_lengths,
                             const char** uniform_names = nullptr,
                             GLuint* uniform_locations = nullptr,
                             const unsigned int uniform_count = 0) noexcept;

#endif // UINTA_SHADER_H
