#ifndef UINTA_QUAD_TREE_H
#define UINTA_QUAD_TREE_H

#include <vec.h>
#include <entt/entt.hpp>
#include <iostream>

struct quad final {
    const vec2d topLeftBounds;
    const vec2d bottomRightBounds;
    entt::entity entities[16];
    char entityCount;

    quad *topLeft;
    quad *topRight;
    quad *bottomLeft;
    quad *bottomRight;

    quad() : quad(vec2d(0.0, 0.0), vec2d(0.0, 0.0))  {
    }

    quad(const vec2d &topLeftBounds, const vec2d &bottomRightBounds) : topLeftBounds(vec2d(topLeftBounds)), bottomRightBounds(vec2d(bottomRightBounds)) {
        topLeft = nullptr;
        topRight = nullptr;
        bottomLeft = nullptr;
        bottomRight = nullptr;
        entityCount = 0;
    }

    void insert(entt::entity entity, const vec2d &pos) {
    }

    void remove(entt::entity entity, const vec2d &pos) {
    }

    const entt::entity* get(const vec2d &pos, char* count) const {
        return nullptr;
    }

    bool isInBounds(const vec2d &pos) const {
        return pos >= topLeftBounds && pos <= bottomRightBounds;
    }

};

#endif // UINTA_QUAD_TREE_H
