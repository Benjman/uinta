#include <cmath>
#include <glad/glad.h>

#include <cstdio>
#include <cstring>

#include <camera2d.hpp>
#include <shader.hpp>

#include "./camera_2drunner.hpp"

void camera2DRunner::init() {
    glfw.width = 1920;
    glfw.height = 1080;
    glfw.title = "hello 2d camera";
    createGLFWWindow(glfw);

    init_shader();
    init_buffers();
    init_mesh();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void camera2DRunner::render() {
    // update the model-view-matrix
    get_view_matrix(camera, view_matrix);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, view_matrix.values);

    
    float aspect_ratio = (float) glfw.width / (float) glfw.height;
    float horizontal = aspect_ratio > 1.0 ? aspect_ratio / camera.scale.x : camera.scale.x;
    float vertical = aspect_ratio < 1.0 ? aspect_ratio / camera.scale.y : camera.scale.y;

    // FIXME when aspect ratio < 1 things get all fucked up
    get_ortho_matrix(proj_matrix, -horizontal, horizontal, vertical, -vertical, 0.1, 1.0);
    // get_perspective_matrix(proj_matrix, camera.fov, view.width, view.height, 0.1, 10.0);
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, proj_matrix.values);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void camera2DRunner::tick(float dt) {
    // camera.pos.x = std::sin(dt);
    // camera.pos.y = std::cos(dt);
    camera.scale.x = (std::cos(dt) + 1) * 0.5;
    camera.scale.y = (std::cos(dt) + 1) * 0.5;
    // camera.scale.x = std::cos(dt);
    // camera.scale.y = std::cos(dt);
}

void camera2DRunner::init_buffers() {
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

void camera2DRunner::init_mesh() {
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

void camera2DRunner::init_shader() {
    const char *vshader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "layout (location = 1) in vec3 in_color;"
        "uniform mat4 u_view;"
        "uniform mat4 u_proj;"
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
