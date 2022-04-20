#include<glad/glad.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <file.hpp>
#include <font.hpp>
#include <shader.hpp>

#include "./runner.hpp"

void showcaseRunner::init() {
    view.width = WINDOW_WIDTH;
    view.height = WINDOW_HEIGHT;
    view.title = "hello showcase";
    createGLFWWindow(view);

    init_shader();
    init_buffers();
    init_mesh();
    init_font();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void showcaseRunner::render() {
    glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);
}

void showcaseRunner::tick(float dt) {
}

void showcaseRunner::init_buffers() {
    glGenVertexArrays(1, &vao);

    GLuint ids[2];
    glGenBuffers(2, ids);
    vbo.id = ids[0];
    ebo.id = ids[1];

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}

using namespace font;
void showcaseRunner::init_mesh() {
    const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib> attribs = {
        {FontMeshAttrib_Position, font_mesh_attrib(7, 0)},
        {FontMeshAttrib_UV, font_mesh_attrib(7, 2)},
        {FontMeshAttrib_Color, font_mesh_attrib(7, 4)},
    };
    unsigned int vbuf_count = 0;
    unsigned int ioffset = 0;

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VBUF_SIZE, vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IBUF_SIZE, ibuf, GL_STATIC_DRAW);
}

void showcaseRunner::init_font() {
}

void showcaseRunner::init_shader() {
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
    shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum));
    glUseProgram(shader);
}

void showcaseRunner::key_callback(int key, int scancode, int action, int mods) noexcept {
}

