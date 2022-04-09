#ifndef UINTA_RUNNER_H
#define UINTA_RUNNER_H

#include <font.hpp>
#include <glfw.hpp>

const unsigned int VBUF_SIZE = 15 * 1024;
const unsigned int IBUF_SIZE = 15 * 1024;

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;


struct showcaseRunner {
public:
    font::font_ctx font = font::font_ctx(font::DejaVuSans, 256, 256);

    // vertex buffer
    float vbuf[VBUF_SIZE];

    // index buffer
    unsigned int ibuf[IBUF_SIZE];
    unsigned int icount = 0;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint shader;

    viewport view;

    void init();

    void render(); 

    void tick(float dt);

    void key_callback(int key, int scancode, int action, int mods) noexcept;

private:
    void init_buffers();
    void init_mesh();
    void init_shader();
};

#endif // UINTA_RUNNER_H


