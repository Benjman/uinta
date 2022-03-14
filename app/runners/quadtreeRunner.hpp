#ifndef UINTA_RUNNER_QUADTREE_H
#define UINTA_RUNNER_QUADTREE_H

#include <iostream>

#include <runner.h>
#include <quadtree.hpp>
#include <window.hpp>

struct quadtreeRunner : public runner {
    unsigned int width = 1088;
    unsigned int height = 1088;

    float vertices[2048];
    unsigned int indices[2048];

    quad qt = quad(vec2(32), vec2(1056), 16);
    float qt_width = qt.bottomRightBounds.x - qt.topLeftBounds.x;
    float qt_height = qt.bottomRightBounds.y - qt.topLeftBounds.y;

    unsigned int squareSize = 16;
    float squareWidth;
    float squareHeight;

    GLuint uniformColor = 0u;

    unsigned int vertexCount = 0u;
    unsigned int indexCount = 0u;
    unsigned int indexOffset = 0u;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "out vec3 pass_color;"
        "void main() {"
        "   gl_Position = vec4(in_pos.x, in_pos.y, 1.0, 1.0);"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;"
        "uniform vec3 u_color;"
        "void main() {"
        "   FragColor = vec4(u_color, 1.0);"
        "}\0";

    void init() override;

    void render() override; 

    void update(float dt) override;

};

#endif // UINTA_RUNNER_QUADTREE_H
