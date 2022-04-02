#ifndef UINTA_CAMERA_RUNNER_H
#define UINTA_CAMERA_RUNNER_H

#include <font.hpp>
#include <glfw.hpp>

const inline unsigned int BUF_SIZE = 2048;

struct fontRunner {

    font_ctx ctx = font_ctx(FontType::DejaVuSans, 256.0f, 256.0f);

    // vertex buffer
    float vbuf[BUF_SIZE];
    // count of verticies in buffer
    unsigned int vcount = 0u;

    // index buffer
    unsigned int ibuf[BUF_SIZE];

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint shader;

    viewport view;

    void init();
    void init_buffers();
    void init_font();
    void init_mesh();
    void init_shader();

    void render(); 

    void tick(float dt);

    void key_callback(int key, int scancode, int action, int mods) noexcept;

};

#endif // UINTA_CAMERA_RUNNER_H

