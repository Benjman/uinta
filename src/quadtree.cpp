#include <quadtree.h>

#include <exception> // for runtime_error
#include <math.h> // for ceil
#include <memory.h> // for memcpy
#include <iostream> // for cerr
#include <string> // for std::to_string

void validateQuad(const quad &quad) {
    // validate bounds are whole numbers
    if (ceilf(quad.topLeftBounds.x) != quad.topLeftBounds.x)
        throw std::runtime_error("quad.topLeftBounds.x must be an integer.");
    if (ceilf(quad.topLeftBounds.y) != quad.topLeftBounds.y)
        throw std::runtime_error("quad.topLeftBounds.y must be an integer.");
    if (ceilf(quad.bottomRightBounds.x) != quad.bottomRightBounds.x)
        throw std::runtime_error("quad.bottomRightBounds.x must be an integer.");
    if (ceilf(quad.bottomRightBounds.y) != quad.bottomRightBounds.y)
        throw std::runtime_error("quad.bottomRightBounds.y must be an integer.");

    int width = ceilf(quad.bottomRightBounds.x - quad.topLeftBounds.x);
    int height = ceilf(quad.bottomRightBounds.y - quad.topLeftBounds.y);

    if (width < 0 || height < 0)
        throw std::runtime_error("bottomRightBounds must be aligned after the topRightBounds spatially.");

    if (width < quad.minCellSize)
        throw std::runtime_error("quad width must be at least the size of the minimum cell size");
    if (height < quad.minCellSize)
        throw std::runtime_error("quad height must be at least the size of the minimum cell size");

    if (width - height)
        throw std::runtime_error("quad boundaries must be a square");

    if ((width & (width - 1)) != 0
        || (height & (height - 1)) != 0)
        throw std::runtime_error("quad size must be a power of 2");

    if ((quad.minCellSize & (quad.minCellSize - 1)) != 0)
        throw std::runtime_error("quad size must be a power of 2");
}

quad::quad() : quad(vec2(0.0), vec2(MIN_CELL_SIZE)) {}

quad::quad(const vec2 &topLeftBounds, const vec2 &bottomRightBounds, const char minCellSize)
    : topLeftBounds(vec2(topLeftBounds)), bottomRightBounds(vec2(bottomRightBounds)), minCellSize(minCellSize) {
    topLeft = nullptr;
    topRight = nullptr;
    bottomLeft = nullptr;
    bottomRight = nullptr;

    entityStore = nullptr;
    entityCount = 0;
    entityStoreSize = 0;

    validateQuad(*this);
}

quad::~quad() {
    delete entityStore;
    entityStore = nullptr;

    // TODO when pooling is implemented, these deletes shouldn't be happening here.
    delete topLeft;
    topLeft = nullptr;

    delete topRight;
    topRight = nullptr;

    delete bottomLeft;
    bottomLeft = nullptr;

    delete bottomRight;
    bottomRight = nullptr;
}

quad *quad::findQuad(const vec2 &pos) const noexcept {
    if (!isInBounds(pos)) {
        return nullptr;
    }

    if ((topLeftBounds.x + bottomRightBounds.x) / 2.0 >= pos.x) {
        if ((topLeftBounds.y + bottomRightBounds.y) / 2.0 >= pos.y) {
            return topLeft;
        } else {
            return bottomLeft;
        }
    } else {
        if ((topLeftBounds.y + bottomRightBounds.y) / 2.0 >= pos.y) {
            return topRight;
        } else {
            return bottomRight;
        }
    }
}

const entt::entity* quad::get(const vec2 &pos, char* count) const noexcept {
    if (!isInBounds(pos)) {
        *count = 0;
        return nullptr;
    }

    if ((topLeftBounds - bottomRightBounds) <= vec2(MIN_CELL_SIZE)) {
        *count = entityCount;
        return entityStore;
    }

    quad *q = findQuad(pos);

    if (q == nullptr) {
        *count = 0;
        return nullptr;
    }

    return q->get(pos, count);
}

void quad::insert(const entt::entity &entity, const vec2 &pos) noexcept {
    if (!isInBounds(pos))
        return;

    if ((topLeftBounds - bottomRightBounds) <= vec2(MIN_CELL_SIZE)) {
        // store_add(entity);
        return;
    }

    float x = (topLeftBounds.x + bottomRightBounds.x) / 2.0;
    float y = (topLeftBounds.y + bottomRightBounds.y) / 2.0;

    // TODO quad pooling
    if (x >= pos.x) {
        if (y >= pos.y) {
            if (topLeft == nullptr) {
                topLeft = new quad(vec2(topLeftBounds.x, topLeftBounds.y),
                vec2((topLeftBounds.x + bottomRightBounds.x) / 2.0,
                (topLeftBounds.y + bottomRightBounds.y) / 2.0));
            }
            topLeft->insert(entity, pos);
        } else {
            if (bottomLeft == nullptr) {
                bottomLeft = new quad(vec2(topLeftBounds.x, (topLeftBounds.y + bottomRightBounds.y) / 2.0),
                vec2((topLeftBounds.x + bottomRightBounds.x) / 2.0, bottomRightBounds.y));
            }
            bottomLeft->insert(entity, pos);
        }
    } else {
        if (y >= pos.y) {
            if (topRight == nullptr) {
                topRight = new quad(vec2((topLeftBounds.x + bottomRightBounds.x) / 2.0, topLeftBounds.y),
                vec2(bottomRightBounds.x, (topLeftBounds.y + bottomRightBounds.y) / 2.0));
            }
            topRight->insert(entity, pos);
        } else {
            if (bottomRight == nullptr) {
                bottomRight = new quad(vec2((topLeftBounds.x + bottomRightBounds.x) / 2.0, (topLeftBounds.y + bottomRightBounds.y) / 2.0),
                vec2(bottomRightBounds.x, bottomRightBounds.y));
            }
            bottomRight->insert(entity, pos);
        }
    }
}

bool quad::isInBounds(const vec2 &pos) const noexcept {
    return pos >= topLeftBounds && pos <= bottomRightBounds;
}

void quad::addEntity(entt::entity entity) {
    if (entityStoreSize == 0 ||
        entityCount + 1 == entityStoreSize) {
        auto *arr = new entt::entity[entityStoreSize + quad::ENTITY_STORE_SIZE_STEP];
        entityStoreSize += quad::ENTITY_STORE_SIZE_STEP;

        if (entityCount > 0)
            memcpy(arr, entityStore, sizeof(entt::entity) * entityCount);

        delete entityStore;
        entityStore = arr;
    }

    entityStore[entityCount] = entity;
    entityCount++;
}

void quad::removeEntity(entt::entity entity) {
    for (int i = 0; i < entityCount; i++) {
        if (entityStore[i] == entity) {
            entityCount--;
            entityStore[i] = entityStore[entityCount];
            return;
        }
    }
    std::cerr << "Entity " << std::to_string((int) entity) << " not found in store.\n"; // TODO logging
}
