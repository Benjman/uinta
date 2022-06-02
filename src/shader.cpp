#include <cstdio>
#include <glad/glad.h>

#include <uinta/logging.hpp>
#include <uinta/macros.hpp>
#include <uinta/shader.hpp>

void checkCompileErrors(const GLuint shader, const GLenum type) noexcept;

GLuint createShaderProgram(const char** sources,
                           const GLenum* stages,
                           const unsigned int stage_count,
                           const GLint* buffer_lengths,
                           const char** uniform_names,
                           GLuint** uniform_locations,
                           const unsigned int uniform_count) noexcept {
    GLuint id = glCreateProgram();

    for (auto i = 0; i < stage_count; i++) {
        GLuint stageId = glCreateShader(stages[i]);
        glShaderSource(stageId, 1, &sources[i], &buffer_lengths[i]);
        glCompileShader(stageId);
        checkCompileErrors(id, stageId);
        glAttachShader(id, stageId);
        glDeleteShader(stageId);
    }

    glLinkProgram(id);
    checkCompileErrors(id, GL_LINK_STATUS);
    glUseProgram(id);

    for (auto i = 0; i < uniform_count; i++) {
        GLuint loc = glGetUniformLocation(id, uniform_names[i]);
        if (loc == -1)
            SPDLOG_ERROR("createShaderProgram - Uniform '{}' not found.", uniform_names[i]);
        *uniform_locations[i] = loc;
    }

    return id;
}

void checkCompileErrors(const GLuint shader, const GLenum type) noexcept {
    GLint success = 1;
    GLchar info[KILOBYTES(1)];

    if (type == GL_LINK_STATUS) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
            glGetProgramInfoLog(shader, KILOBYTES(1), 0, info);
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
            glGetShaderInfoLog(shader, KILOBYTES(1), 0, info);
    }
    if (!success)
        SPDLOG_ERROR("{}", info);
}
