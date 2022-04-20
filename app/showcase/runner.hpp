#ifndef UINTA_RUNNER_H
#define UINTA_RUNNER_H

#include <glfw.hpp>
#include <buffer.hpp>

const unsigned int VBUF_SIZE = 15 * 1024;
const unsigned int IBUF_SIZE = 15 * 1024;

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;


struct showcaseRunner {
public:
    GLfloat vbuf[VBUF_SIZE];
    GLuint ibuf[IBUF_SIZE];

    GLuint vao;
    GLuint shader;

    gl_buf vbo;
    gl_buf ebo;

    viewport view;

    void init();

    void render(); 

    void tick(float dt);

    void key_callback(int key, int scancode, int action, int mods) noexcept;

private:
    void init_buffers();
    void init_mesh();
    void init_shader();
    void init_font();

};

#endif // UINTA_RUNNER_H


