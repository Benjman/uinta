#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

#include <file.hpp>
#include <macros.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <shader.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

struct modelRunner final : glfw_runner {
    unsigned int icount = 0, vcount = 0;

    GLuint shader, u_model;
    float vbuf[MEGABYTES(5)];
    unsigned int ibuf[MEGABYTES(5)];

    modelRunner() : glfw_runner("hello models", 1000, 1000) {}

    void doInit() override {
        load_shaders();
        init_obj();
        init_buffers();
        glEnable(GL_DEPTH_TEST);
        setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void doPreTick(const RunnerState& state) override {
        if (state.input.isKeyPressed(KEY_SPACE)) {
            glDeleteProgram(shader);
            load_shaders();
            glUseProgram(shader);
        }
    }

    void init_obj() {
        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, mesh_attrib(6, 0)},
            {MeshAttribType_Normal, mesh_attrib(6, 3)},
        };
        loadObj(Model_Suzanne, vbuf, &vcount, ibuf, &icount, &attribs);
    }

    void init_buffers() {
        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vbuf), vbuf, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuf), ibuf, GL_STATIC_DRAW);
    }

    void load_shaders() {
        char vert[get_file_size("shader/model.vert")];
        read_file_raw("shader/model.vert", vert);

        char frag[get_file_size("shader/model.frag")];
        read_file_raw("shader/model.frag", frag);

        const char* sources[] = { vert, frag };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const GLint buffer_lengths[] = { (GLint) sizeof(vert), (GLint) sizeof(frag) };
        const char* uniforms[] = { "u_model" };
        GLuint* uniform_locations[] = { &u_model };
        shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), buffer_lengths,
                                       uniforms, uniform_locations, sizeof(uniforms) / sizeof(char*));
    }

    void doRender() override {
        glm::mat4 mat = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &mat[0][0]);
        glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
    }

};

modelRunner runner;

int main(const int argc, const char **argv) {
    return runner.run();
}
