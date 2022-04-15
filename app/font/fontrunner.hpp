#ifndef UINTA_CAMERA_RUNNER_H
#define UINTA_CAMERA_RUNNER_H

#include <font.hpp>
#include <glfw.hpp>

const unsigned int VBUF_SIZE = 15 * 1024;
const unsigned int IBUF_SIZE = 15 * 1024;

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;


struct fontRunner {
public:
    font::font_ctx font = font::font_ctx(font::DejaVuSans, 256, 256);
    font::text text = font::text(&font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
                                 32.0, // line_size
                                 1.0, 0.5, 0.2, // color
                                 0.0, 0.0, // pos
                                 WINDOW_WIDTH * 0.75, 0.0 // max dimensions
                                 );

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
    void init_font();
};

#endif // UINTA_CAMERA_RUNNER_H

