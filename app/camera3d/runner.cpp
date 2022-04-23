#include <glad/glad.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <file.hpp>
#include <glm/trigonometric.hpp>
#include <shader.hpp>

#include "./runner.hpp"

void camera3dRunner::init() {
    view.width = WINDOW_WIDTH;
    view.height = WINDOW_HEIGHT;
    view.title = "hello 3D camera";
    createGLFWWindow(view);

    init_shader();
    init_buffers();
    init_mesh();

    cam.pos_z.force(3.0);
    cam.pos_y.target = -3.0;

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void camera3dRunner::render() {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader);


    tmp_mat = glm::scale(glm::mat4(1.0), glm::vec3(3.0, 0.125, 3.0));
    tmp_mat = glm::translate(tmp_mat, glm::vec3(0.0, 0.0, -1.5));
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &tmp_mat[0][0]);

    get_view_matrix(
            glm::vec3(cam.pos_x, cam.pos_y, cam.pos_z),
            cam.attitude,
            &tmp_mat);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &tmp_mat[0][0]);

    tmp_mat = glm::perspective((float) glm::radians(90.0), (float) view.width / (float) view.height, 0.1f, 10.0f);
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, &tmp_mat[0][0]);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);

    glDrawArrays(GL_TRIANGLES, 0, vbo.count);
}

void camera3dRunner::tick(float dt) {
    runtime += dt;
    cam.pos_x.update(dt);
    cam.pos_y.update(dt);
    cam.pos_z.update(dt);
}

void camera3dRunner::init_buffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo.id);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void camera3dRunner::init_mesh() {
    GLfloat vertices[] = {
        -1.0, -1.0, -1.0,  1.0, 0.0, 0.0,
         1.0, -1.0, -1.0,  1.0, 0.0, 0.0,
         1.0,  1.0, -1.0,  1.0, 0.0, 0.0,
         1.0,  1.0, -1.0,  1.0, 0.0, 0.0,
        -1.0,  1.0, -1.0,  1.0, 0.0, 0.0,
        -1.0, -1.0, -1.0,  1.0, 0.0, 0.0,

        -1.0, -1.0,  1.0,  0.0, 1.0, 0.0,
         1.0, -1.0,  1.0,  0.0, 1.0, 0.0,
         1.0,  1.0,  1.0,  0.0, 1.0, 0.0,
         1.0,  1.0,  1.0,  0.0, 1.0, 0.0,
        -1.0,  1.0,  1.0,  0.0, 1.0, 0.0,
        -1.0, -1.0,  1.0,  0.0, 1.0, 0.0,

        -1.0,  1.0,  1.0,  0.0, 0.0, 1.0,
        -1.0,  1.0, -1.0,  0.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,  0.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,  0.0, 0.0, 1.0,
        -1.0, -1.0,  1.0,  0.0, 0.0, 1.0,
        -1.0,  1.0,  1.0,  0.0, 0.0, 1.0,

         1.0,  1.0,  1.0,  1.0, 1.0, 0.0,
         1.0,  1.0, -1.0,  1.0, 1.0, 0.0,
         1.0, -1.0, -1.0,  1.0, 1.0, 0.0,
         1.0, -1.0, -1.0,  1.0, 1.0, 0.0,
         1.0, -1.0,  1.0,  1.0, 1.0, 0.0,
         1.0,  1.0,  1.0,  1.0, 1.0, 0.0,

        -1.0, -1.0, -1.0,  1.0, 0.0, 1.0,
         1.0, -1.0, -1.0,  1.0, 0.0, 1.0,
         1.0, -1.0,  1.0,  1.0, 0.0, 1.0,
         1.0, -1.0,  1.0,  1.0, 0.0, 1.0,
        -1.0, -1.0,  1.0,  1.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,  1.0, 0.0, 1.0,

        -1.0,  1.0, -1.0,  0.0, 1.0, 1.0,
         1.0,  1.0, -1.0,  0.0, 1.0, 1.0,
         1.0,  1.0,  1.0,  0.0, 1.0, 1.0,
         1.0,  1.0,  1.0,  0.0, 1.0, 1.0,
        -1.0,  1.0,  1.0,  0.0, 1.0, 1.0,
        -1.0,  1.0, -1.0,  0.0, 1.0, 1.0
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vbo.count = 36;
}

void camera3dRunner::init_shader() {
    const char *vshader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 in_pos;"
        "layout (location = 1) in vec3 in_color;"
        "uniform mat4 u_model = mat4(1.0);"
        "uniform mat4 u_view = mat4(1.0);"
        "uniform mat4 u_proj = mat4(1.0);"
        "out vec3 pass_color;"
        "void main() {"
        "  pass_color = in_color;"
        "  gl_Position = u_proj * u_view * u_model * vec4(in_pos, 1.0);"
        "}\0";

    const char *fshader =
        "#version 330 core\n"
        "in vec3 pass_color;"
        "void main() {"
        "  gl_FragColor = vec4(pass_color, 1.0);"
        "}\0";

    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    const char* uniforms[] = { "u_model", "u_view", "u_proj" };
    GLuint uniform_locs[3];
    shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum),
                                   uniforms, uniform_locs, sizeof(uniform_locs) / sizeof(GLuint));
    u_model = uniform_locs[0];
    u_view = uniform_locs[1];
    u_proj = uniform_locs[2];
}

void camera3dRunner::key_callback(int key, int scancode, int action, int mods) noexcept {
    if (action == GLFW_PRESS && key == GLFW_KEY_E)
        cam.pos_y += 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_D)
        cam.pos_y -= 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_S)
        cam.pos_x -= 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_F)
        cam.pos_x += 1.0;
}

