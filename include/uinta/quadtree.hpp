#ifndef UINTA_QUAD_TREE_H
#define UINTA_QUAD_TREE_H

#include <uinta/math.hpp>

#include <entt/entity/fwd.hpp>

static const inline char QUAD_MIN_CELL_SIZE          = 1;
static const inline char QUAD_ENTITY_STORE_SIZE_STEP = 2;

/**
    TODO So far I can see a practical advantage to only having a single entity per cell. Convert to single entity.
    TODO quads should be POD objects. All the functional aspects of it should be taken out of this class and converted into a
functional interface.
**/

#include <glm/vec2.hpp>
struct Quad final {
  Quad *parent;
  Quad *bottomLeft;
  Quad *bottomRight;
  Quad *topLeft;
  Quad *topRight;

  entt::entity *entityStore = nullptr;

  glm::vec2 topLeftBounds;
  const glm::vec2 bottomRightBounds;

  const unsigned int minCellSize;
  unsigned char entityStoreSize;
  unsigned char entityCount;

  Quad();

  Quad(const glm::vec2 &topLeftBounds, const glm::vec2 &bottomRightBounds, const unsigned int minCellSize = QUAD_MIN_CELL_SIZE);

  ~Quad();

  Quad *findQuad(const glm::vec2 &pos) const noexcept;

  const entt::entity *get(const glm::vec2 &pos, char *count) const noexcept;

  void insert(const entt::entity &entity, const glm::vec2 &pos) noexcept;

  bool isInBounds(const glm::vec2 &pos) const noexcept;

  bool isActive() const noexcept;

  void clear() noexcept;

private:
  void addEntity(entt::entity entity) noexcept;

  void removeEntity(entt::entity entity) noexcept;

  void removeQuad(const Quad *quad) noexcept;
};

#endif // UINTA_QUAD_TREE_H
