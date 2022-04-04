#include <cstdio>
#include <glad/glad.h>
#include <shader.hpp>

void checkCompileErrors(const GLuint shader, const GLenum type) noexcept;

GLuint create_shader_program(const char** sources,
                             const GLenum* stages,
                             const unsigned int stage_count,
                             const char** uniform_names,
                             GLuint* uniform_locations,
                             const unsigned int uniform_count) noexcept {
    GLuint id = glCreateProgram();

    for (auto i = 0; i < stage_count; i++) {
        GLuint stageId = glCreateShader(stages[i]);
        glShaderSource(stageId, 1, &sources[i], nullptr);
        glCompileShader(stageId);
        checkCompileErrors(id, stageId);
        glAttachShader(id, stageId);
        glDeleteShader(stageId);
    }

    glLinkProgram(id);
    checkCompileErrors(id, GL_LINK_STATUS);

    if (uniform_count > 0) {
        glUseProgram(id);
        for (auto i = 0; i < uniform_count; i++) {
            GLuint loc = glGetUniformLocation(id, uniform_names[i]);
            uniform_locations[i] = glGetUniformLocation(id, uniform_names[i]);
        }
    }

    return id;
}

void checkCompileErrors(const GLuint shader, const GLenum type) noexcept {
    GLint success = 1;
    GLchar info[1024];
    if (type == GL_LINK_STATUS) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
            glGetProgramInfoLog(shader, 1024, 0, info);
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
            glGetShaderInfoLog(shader, 1024, 0, info);
    }
    if (!success)
        printf("Shader error: %s\n", info);
}
