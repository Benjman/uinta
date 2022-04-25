#ifndef UINTA_RUNNER_H
#define UINTA_RUNNER_H

#include <glm/glm.hpp>

#include <glfw.hpp>
#include <buffer.hpp>
#include <camera.hpp>

const unsigned int VBUF_SIZE = 15 * 1024;
const unsigned int IBUF_SIZE = 15 * 1024;

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

struct camera3dRunner {
public:
    camera cam;

    GLuint u_mvp;

    float runtime = 0.0;
    GLuint shader;
    GLuint vao;
    gl_buf vbo;
    viewport view;

    void init();
    void init_buffers();
    void init_mesh();
    void init_shader();

    void render(); 

    void tick(float dt);

    void key_callback(int key, int scancode, int action, int mods) noexcept;

};

#endif // UINTA_RUNNER_H


