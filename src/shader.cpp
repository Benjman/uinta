#include <glad/glad.h>
#include <shader.hpp>

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
        glAttachShader(id, stageId);
        glDeleteShader(stageId);
    }

    glLinkProgram(id);

    if (uniform_count > 0) {
        glUseProgram(id);
        for (auto i = 0; i < uniform_count; i++) {
            GLuint loc = glGetUniformLocation(id, uniform_names[i]);
            uniform_locations[i] = glGetUniformLocation(id, uniform_names[i]);
        }
    }

    return id;
}
