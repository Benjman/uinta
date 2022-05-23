#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include <font.hpp>
#include <glfw.hpp>
#include <macros.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

const unsigned int VBUF_SIZE = KILOBYTES(15);
const unsigned int IBUF_SIZE = KILOBYTES(15);

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

struct fontRunner final : runner {
public:
    font::font_t font_handle;
    font::text text = font::text("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
                                 32.0, // line_size
                                 1.0, 0.5, 0.2, // color
                                 0.0, 0.0, // pos
                                 WINDOW_WIDTH * 0.75, 0.0 // max dimensions
                                 );

    // vertex buffer
    float vbuf[VBUF_SIZE];

    // index buffer
    unsigned int ibuf[IBUF_SIZE];
    unsigned int icount = 0;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint shader;

    fontRunner() : runner("hello font", WINDOW_WIDTH, WINDOW_HEIGHT) {}

    void doInit() override {
        init_font();
        init_shader();
        init_buffers();
        init_mesh();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void doRender() override {
        glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
    }

    void init_buffers() {
        glGenVertexArrays(1, &vao);

        GLuint ids[2];
        glGenBuffers(2, ids);
        vbo = ids[0];
        ebo = ids[1];

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (4 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    void init_mesh() {
        const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib> attribs = {
            {FontMeshAttrib_Position, font_mesh_attrib(7, 0)},
            {FontMeshAttrib_UV, font_mesh_attrib(7, 2)},
            {FontMeshAttrib_Color, font_mesh_attrib(7, 4)},
        };
        unsigned int vbuf_count = 0;
        unsigned int ioffset = 0;
        font::generate_mesh(&text, font_handle, view.width, view.height, &attribs, vbuf, &vbuf_count, ibuf, &icount, &ioffset);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VBUF_SIZE, vbuf, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IBUF_SIZE, ibuf, GL_STATIC_DRAW);
    }

    void init_shader() {
        const char *vshader =
            "#version 330 core\n"
            "layout (location = 0) in vec2 in_pos;"
            "layout (location = 1) in vec2 in_uv;"
            "layout (location = 2) in vec3 in_color;"
            "out vec2 pass_uv;"
            "out vec3 pass_color;"
            "void main() {"
            "  pass_uv = in_uv;"
            "  pass_color = in_color;"
            "  gl_Position = vec4(in_pos, 0.0, 1.0);"
            "}\0";

        const char *fshader =
            "#version 330 core\n"
            "in vec2 pass_uv;"
            "in vec3 pass_color;"
            "uniform sampler2D atlas;"
            "out vec4 out_color;"
            "void main() {"
            "  out_color = vec4(pass_color, texture(atlas, pass_uv).r);"
            "}\0";

        const char* sources[] = { vshader, fshader };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const GLint buffer_lengths[] = { (GLint) strlen(vshader), (GLint) strlen(fshader) };
        shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), buffer_lengths);
        glUseProgram(shader);
    }

    void init_font() {
        auto type = font::DejaVuSans;
        font_handle = font::init_font(type, 256, 256);
        unsigned char data[get_file_size(getFontPath(type))];
        read_file_binary(getFontPath(type), (char*) data);
        load_font(font_handle, data);
    }

};

fontRunner runner;

int main(const int argc, const char **argv) {
    runner.init();

    glfwSetKeyCallback(runner.view.window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        runner.handleKeyInput(key, scancode, action, mods);
    });

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();
        while (!runner.shouldRenderFrame())
            runner.tick(glfwGetTime());
        runner.render();
    }

    runner.shutdown();
    on_exit([] (int status, void* arg) {
        if (runner.view.window)
            glfwDestroyWindow(runner.view.window);
        glfwTerminate();
    }, nullptr);
    return 0;
}
