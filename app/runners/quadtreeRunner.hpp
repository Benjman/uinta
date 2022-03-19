#ifndef UINTA_RUNNER_QUADTREE_H
#define UINTA_RUNNER_QUADTREE_H

#include <runner.h>
#include <quadtree.hpp>
#include <entt/entity/registry.hpp>

struct quadtreeRunner : public runner {
    unsigned int width = 1088;
    unsigned int height = 1088;

    entt::registry registry;

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

    void init() override;

    void render() override; 

    void update(float dt) override;

};

#endif // UINTA_RUNNER_QUADTREE_H
