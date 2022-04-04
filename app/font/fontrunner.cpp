#include<glad/glad.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <file.hpp>
#include <font.hpp>
#include <shader.hpp>

#include "./fontrunner.hpp"

void fontRunner::init() {
    view.width = 1000;
    view.height = 1000;
    view.title = "hello font";
    createGLFWWindow(view);

    init_font();
    init_shader();
    init_buffers();
    init_mesh();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void fontRunner::render() {
    glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
}

void fontRunner::tick(float dt) {
}

void fontRunner::init_buffers() {
    glGenVertexArrays(1, &vao);

    GLuint ids[2];
    glGenBuffers(2, ids);
    vbo = ids[0];
    ebo = ids[1];

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void fontRunner::init_font() {
    load_font(ctx);
}

void fontRunner::init_mesh() {
    const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
        {MeshAttribType_Position, mesh_attrib(2, 4, 0)},
        {MeshAttribType_UV, mesh_attrib(2, 4, 2)},
    };
    unsigned int vbuf_count = 0;
    unsigned int ioffset = 0;
    font::generate_mesh(&text, view.width, view.height, 32.0, attribs, vbuf, &vbuf_count, ibuf, &icount, &ioffset);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VBUF_SIZE, vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IBUF_SIZE, ibuf, GL_STATIC_DRAW);

    // const float mesh[] = {
    //     // atlas
    //     -1.0, 1.0, 0.0, 1.0, // top-left
    //     -1.0, 0.0, 0.0, 0.0, // bottom-left
    //      0.0, 0.0, 1.0, 0.0, // bottom-right
    //      0.0, 1.0, 1.0, 1.0, // top-right
    //
    //     // quad
    //     -1.0,  0.0, quad.s0, -quad.t0, // top-left
    //     -1.0, -1.0, quad.s0, -quad.t1, // bottom-left
    //      0.0, -1.0, quad.s1, -quad.t1, // bottom-right
    //      0.0,  0.0, quad.s1, -quad.t0, // top-right
    // };
    // memcpy(vbuf, mesh, sizeof(mesh));
    // vcount = 4;
    //
    // unsigned int indices[] = {
    //     // atlas
    //     0, 1, 2,
    //     2, 3, 0,
    //
    //     // quad
    //     4, 5, 6,
    //     6, 7, 4,
    // };
    // memcpy(ibuf, indices, sizeof(indices));
    //
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * BUF_SIZE, vbuf, GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * BUF_SIZE, ibuf, GL_STATIC_DRAW);
}

void fontRunner::init_shader() {
    const char *vshader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "layout (location = 1) in vec2 in_uv;"
        "out vec2 pass_uv;"
        "void main() {"
        "  pass_uv = in_uv;"
        "  gl_Position = vec4(in_pos, 0.0, 1.0);"
        "}\0";

    const char *fshader =
        "#version 330 core\n"
        "in vec2 pass_uv;"
        "uniform sampler2D atlas;"
        "out vec4 out_color;"
        "void main() {"
        "  out_color = vec4(1.0, 1.0, 1.0, texture(atlas, pass_uv).r);"
        "}\0";

    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum));
    glUseProgram(shader);
}

void fontRunner::key_callback(int key, int scancode, int action, int mods) noexcept {
}
