#ifndef UINTA_RUNNER_QUADTREE_H
#define UINTA_RUNNER_QUADTREE_H

#include <buffer.hpp>
#include <glfw.hpp>
#include <quadtree.hpp>
#include <gl.h>

struct quadtreeRunner {
    unsigned int width = 1088;
    unsigned int height = 1088;

    GLfloat vertices[2048];
    GLuint indices[2048];

    quad qt = quad(vec2(32), vec2(1056), 16);
    float qt_width = qt.bottomRightBounds.x - qt.topLeftBounds.x;
    float qt_height = qt.bottomRightBounds.y - qt.topLeftBounds.y;

    unsigned int squareSize = 16;
    float squareWidth;
    float squareHeight;

    GLuint uniformColor = 0u;

    gl_buf vbo;
    gl_buf ebo;

    viewport view;

    void init();
    void init_buffers();
    void init_shader();

    void render(); 

    void tick(float runtime);

};

#endif // UINTA_RUNNER_QUADTREE_H
