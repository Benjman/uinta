#include <cmath>
#include <glad/glad.h>

#include <cstdio>
#include <cstring>

#include <camera2d.hpp>

#include "./camera_2drunner.hpp"

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

void generate_square(float *vbuf, unsigned int *vcount, unsigned int *ibuf, unsigned int *icount, unsigned int *ioff);
void create_gpu_buffers(GLuint *vao, GLuint *vbo, GLuint *ebo);
void create_shader(GLuint *shader, GLint *u_view, GLint *u_proj);

void camera2DRunner::init() {
    createWindow(1920, 1080, "hello 2d camera");
    create_gpu_buffers(&vao, &vbo, &ebo);
    generate_square(vbuf, &vcount, ibuf, &icount, &ioff);
    create_shader(&shader, &u_view, &u_proj);
}

void camera2DRunner::render() {
    // update the model-view-matrix
    get_view_matrix(camera, view_matrix);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, view_matrix.values);

    
    float aspect_ratio = (float) view.width / (float) view.height;
    float horizontal = aspect_ratio > 1.0 ? aspect_ratio : 1.0;
    float vertical = aspect_ratio <= 1.0 ? aspect_ratio : 1.0;
    get_ortho_matrix(proj_matrix, -horizontal, horizontal, vertical, -vertical, 0.1, 1.0);
    // get_perspective_matrix(proj_matrix, camera.fov, view.width, view.height, 0.1, 10.0);
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, proj_matrix.values);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void camera2DRunner::tick(float dt) {
    camera.pos.x = std::sin(dt);
    camera.pos.y = std::cos(dt);
    // camera.scale.x = std::cos(dt);
    // camera.scale.y = std::cos(dt);
}

void generate_square(float *vbuf, unsigned int *vcount, unsigned int *ibuf, unsigned int *icount, unsigned int *ioff) {
    float tmpv[] = {
        -0.5,  0.5, 1.0, 0.5, 0.2,
        -0.5, -0.5, 1.0, 0.5, 0.2,
         0.5, -0.5, 1.0, 0.5, 0.2,
         0.5,  0.5, 1.0, 0.5, 0.2,
    };
    memcpy(vbuf, tmpv, sizeof(tmpv));
    *vcount = 4;

    unsigned int tmpi[] = {
        0, 1, 2,
        2, 3, 0
    };
    memcpy(ibuf, tmpi, sizeof(tmpi));
    *icount = 6;
    *ioff = 4;

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * BUF_SIZE, vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * BUF_SIZE, ibuf, GL_STATIC_DRAW);
}

void create_gpu_buffers(GLuint *vao, GLuint *vbo, GLuint *ebo) {
    glGenVertexArrays(1, vao);
    GLuint tmp[2];
    glGenBuffers(2, tmp);
    *vbo = tmp[0];
    *ebo = tmp[1];

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void create_shader(GLuint *shader, GLint* u_view, GLint* u_proj) {
    GLuint vid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vid, 1, &vshader, NULL);
    glShaderSource(fid, 1, &fshader, NULL);

    glCompileShader(vid);
    glCompileShader(fid);

    *shader = glCreateProgram();
    glAttachShader(*shader, vid);
    glAttachShader(*shader, fid);
    glLinkProgram(*shader);
    glDeleteShader(vid);
    glDeleteShader(fid);

    glUseProgram(*shader);

    *u_view = glGetUniformLocation(*shader, "u_view");
    *u_proj = glGetUniformLocation(*shader, "u_proj");
}
