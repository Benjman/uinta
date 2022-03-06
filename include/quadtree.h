#ifndef UINTA_QUAD_TREE_H
#define UINTA_QUAD_TREE_H

#include <entt/entity/fwd.hpp>
#include <vec.h>

struct quad final {
    static const inline char MIN_CELL_SIZE = 1;
    static const inline char ENTITY_STORE_SIZE_STEP = 2;

    quad *topLeft;
    quad *topRight;
    quad *bottomLeft;
    quad *bottomRight;

    entt::entity *entityStore;

    const vec2 topLeftBounds;
    const vec2 bottomRightBounds;

    const unsigned char minCellSize;
    unsigned char entityStoreSize;
    unsigned char entityCount;

    quad();

    quad(const vec2 &topLeftBounds, const vec2 &bottomRightBounds, const unsigned char minCellSize = MIN_CELL_SIZE);

    ~quad();

    quad *findQuad(const vec2 &pos) const noexcept;

    const entt::entity* get(const vec2 &pos, char* count) const noexcept;

    void insert(const entt::entity &entity, const vec2 &pos) noexcept;

    bool isInBounds(const vec2 &pos) const noexcept;

private:
    void addEntity(entt::entity entity);

    void removeEntity(entt::entity entity);

};

#endif // UINTA_QUAD_TREE_H
