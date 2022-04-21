#include<glad/glad.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <camera.hpp>
#include <shader.hpp>

#include "./camera2drunner.hpp"

void camera2dRunner::init() {
    view.width = 1920;
    view.height = 1080;
    view.title = "hello 2d camera";
    createGLFWWindow(view);

    camera.pos = vec2(0.0);

    init_shader();
    init_buffers();
    init_mesh();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void camera2dRunner::render() {
    get_view_matrix(camera, &view_matrix);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, view_matrix.values);
    
    // FIXME when aspect ratio < 1 things get all fucked up
    float aspect_ratio = (float) view.width / (float) view.height;
    float horizontal = aspect_ratio > 1.0 ? aspect_ratio / camera.ortho_size : camera.ortho_size;
    float vertical = aspect_ratio < 1.0 ? aspect_ratio / camera.ortho_size : camera.ortho_size;

// void get_ortho_matrix(mat4 &mat, float left, float right, float bottom, float top, float near, float far) noexcept {
    get_ortho_matrix(proj_matrix, -horizontal, horizontal, -vertical, vertical, 0.1, 1.0);
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, proj_matrix.values);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void camera2dRunner::tick(float dt) {
    // camera.pos.x = std::sin(dt);
    // camera.pos.y = std::cos(dt);
}

void camera2dRunner::init_buffers() {
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void camera2dRunner::init_mesh() {
    const float cube_mesh[] = {
        -0.25,  0.25, 1.0, 0.5, 0.2,
        -0.25, -0.25, 1.0, 0.5, 0.2,
         0.25, -0.25, 1.0, 0.5, 0.2,
         0.25,  0.25, 1.0, 0.5, 0.2,
    };
    memcpy(vbuf, cube_mesh, sizeof(cube_mesh));
    vcount = 4;

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    memcpy(ibuf, indices, sizeof(indices));
    icount += 6;
    ioff += 4;

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * BUF_SIZE, vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * BUF_SIZE, ibuf, GL_STATIC_DRAW);
}

void camera2dRunner::init_shader() {
    const char *vshader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "layout (location = 1) in vec3 in_color;"
        "uniform mat4 u_view = mat4(1.0);"
        "uniform mat4 u_proj = mat4(1.0);"
        "const mat4 u_model = mat4(1.0);"
        "out vec4 pass_color;"
        "void main() {"
        "  gl_Position = u_proj * u_view * u_model * vec4(in_pos, 1.0, 1.0);"
        "  pass_color = vec4(in_color, 1.0);"
        "}\0";

    const char *fshader =
        "#version 330 core\n"
        "in vec4 pass_color;"
        "out vec4 out_color;"
        "void main() {"
        "  out_color = pass_color;"
        "}\0";

    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    const char* uniforms[] = { "u_view", "u_proj" };
    const unsigned int uniform_count = sizeof(uniforms) / sizeof(const char*);
    GLuint locs[uniform_count];
    shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), uniforms, locs, uniform_count);

    u_view = locs[0];
    u_proj = locs[1];
}

void camera2dRunner::key_callback(int key, int scancode, int action, int mods) noexcept {
    if (action == GLFW_PRESS && key == GLFW_KEY_E)
        camera.pos.y += 0.25f;
    if (action == GLFW_PRESS && key == GLFW_KEY_D)
        camera.pos.y -= 0.25f;
    if (action == GLFW_PRESS && key == GLFW_KEY_F)
        camera.pos.x += 0.25f;
    if (action == GLFW_PRESS && key == GLFW_KEY_S)
        camera.pos.x -= 0.25f;
}
