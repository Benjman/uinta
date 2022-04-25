#include <glad/glad.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>

#include <file.hpp>
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

    cam.pos.y = 5.0;
    cam.pos.z = 7.0;
    cam.pitch(30.0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void camera3dRunner::render() {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader);

    glm::mat4 model_matrix, projection_matrix;

    model_matrix = glm::scale(glm::mat4(1.0), glm::vec3(3.0, 0.125, 3.0));

    glm::mat4 view_matrix;
    get_view_matrix(&view_matrix, cam.pos, cam.pitch(), cam.yaw());

    float near_plane = 0.1f,
          far_plane = 100.0f;

    projection_matrix = glm::perspective((float) glm::radians(90.0), (float) view.width / (float) view.height, near_plane, far_plane);
    // projection_matrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, near_plane, far_plane);

    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(projection_matrix * view_matrix * model_matrix)[0][0]);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);

    glDrawArrays(GL_TRIANGLES, 0, vbo.count);
}

void camera3dRunner::tick(float dt) {
    runtime += dt;
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
        // positions        // colors
        -1.0, -1.0, -1.0,   1.0, 0.0, 0.0,
         1.0, -1.0, -1.0,   1.0, 0.0, 0.0,
         1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
         1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
        -1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
        -1.0, -1.0, -1.0,   1.0, 0.0, 0.0,

        -1.0, -1.0,  1.0,   0.0, 1.0, 0.0,
         1.0, -1.0,  1.0,   0.0, 1.0, 0.0,
         1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
         1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
        -1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
        -1.0, -1.0,  1.0,   0.0, 1.0, 0.0,

        -1.0,  1.0,  1.0,   0.0, 0.0, 1.0,
        -1.0,  1.0, -1.0,   0.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,   0.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,   0.0, 0.0, 1.0,
        -1.0, -1.0,  1.0,   0.0, 0.0, 1.0,
        -1.0,  1.0,  1.0,   0.0, 0.0, 1.0,

         1.0,  1.0,  1.0,   1.0, 1.0, 0.0,
         1.0,  1.0, -1.0,   1.0, 1.0, 0.0,
         1.0, -1.0, -1.0,   1.0, 1.0, 0.0,
         1.0, -1.0, -1.0,   1.0, 1.0, 0.0,
         1.0, -1.0,  1.0,   1.0, 1.0, 0.0,
         1.0,  1.0,  1.0,   1.0, 1.0, 0.0,

        -1.0, -1.0, -1.0,   1.0, 0.0, 1.0,
         1.0, -1.0, -1.0,   1.0, 0.0, 1.0,
         1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
         1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
        -1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
        -1.0, -1.0, -1.0,   1.0, 0.0, 1.0,

        -1.0,  1.0, -1.0,   0.0, 1.0, 1.0,
         1.0,  1.0, -1.0,   0.0, 1.0, 1.0,
         1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
         1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
        -1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
        -1.0,  1.0, -1.0,   0.0, 1.0, 1.0
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
        "uniform mat4 u_mvp = mat4(1.0);"
        "out vec3 pass_color;"
        "void main() {"
        "  pass_color = in_color;"
        "  gl_Position = u_mvp * vec4(in_pos, 1.0);"
        "}\0";

    const char *fshader =
        "#version 330 core\n"
        "in vec3 pass_color;"
        "void main() {"
        "  gl_FragColor = vec4(pass_color, 1.0);"
        "}\0";

    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    const char* uniforms[] = { "u_mvp" };
    shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum),
                                   uniforms, &u_mvp, 1);
}

void camera3dRunner::key_callback(int key, int scancode, int action, int mods) noexcept {
    if (action == GLFW_PRESS && key == GLFW_KEY_E)
        cam.pos.z -= 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_D)
        cam.pos.z += 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_S)
        cam.pos.x -= 1.0;
    if (action == GLFW_PRESS && key == GLFW_KEY_F)
        cam.pos.x += 1.0;
}

