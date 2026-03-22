#include "uinta/scenes/city_game/movement_system.h"

#include <glm/geometric.hpp>

#include "uinta/scenes/city_game/pawn_types.h"

namespace uinta {

constexpr f32 ARRIVAL_THRESHOLD = 0.5f;

// Simple AABB collision check
bool isInsideBuilding(const glm::vec3& pos,
                      const IntersectionBuildings& buildings, size_t b) {
  glm::vec3 center = buildings.positions[b];
  glm::vec3 halfDims = buildings.dimensions[b] * 0.5f;

  return pos.x >= center.x - halfDims.x && pos.x <= center.x + halfDims.x &&
         pos.z >= center.z - halfDims.z && pos.z <= center.z + halfDims.z;
}

// Push position out of buildings
glm::vec3 resolveCollisions(glm::vec3 pos,
                            const IntersectionBuildings& buildings) {
  for (size_t b = 0; b < buildings.count(); ++b) {
    if (!isInsideBuilding(pos, buildings, b)) {
      continue;
    }

    glm::vec3 center = buildings.positions[b];
    glm::vec3 halfDims = buildings.dimensions[b] * 0.5f;

    // Find shortest way out
    f32 pushLeft = (center.x - halfDims.x) - pos.x;
    f32 pushRight = (center.x + halfDims.x) - pos.x;
    f32 pushBack = (center.z - halfDims.z) - pos.z;
    f32 pushFront = (center.z + halfDims.z) - pos.z;

    // Find minimum push distance
    f32 minPushX =
        (std::abs(pushLeft) < std::abs(pushRight)) ? pushLeft : pushRight;
    f32 minPushZ =
        (std::abs(pushBack) < std::abs(pushFront)) ? pushBack : pushFront;

    // Push along shortest axis
    if (std::abs(minPushX) < std::abs(minPushZ)) {
      pos.x += minPushX - 0.1f * (minPushX > 0 ? 1 : -1);
    } else {
      pos.z += minPushZ - 0.1f * (minPushZ > 0 ? 1 : -1);
    }
  }
  return pos;
}

void movementSystem(Pawns& pawns, IntersectionBuildings& buildings,
                    f32 dt) noexcept {
  for (size_t i = 0; i < pawns.count(); ++i) {
    // Skip pawns not walking
    if (pawns.states[i] != PawnState::Walking) {
      continue;
    }

    glm::vec3 pos = pawns.positions[i];
    glm::vec3 dest = pawns.destinations[i];
    glm::vec3 diff = dest - pos;
    f32 dist = glm::length(diff);

    // Check for arrival
    if (dist < ARRIVAL_THRESHOLD) {
      pawns.positions[i] = dest;

      // Enter the target building
      BuildingId arrivedAt = pawns.targetBuilding[i];
      if (arrivedAt != NULL_BUILDING) {
        pawns.currentBuilding[i] = arrivedAt;

        // Transition to appropriate state based on building type
        BuildingType type = buildings.types[arrivedAt];
        if (type == BuildingType::Workplace && pawns.jobs[i] == arrivedAt) {
          pawns.states[i] = PawnState::Working;
          buildings.currentOccupants[arrivedAt]++;
        } else if (type == BuildingType::Shop) {
          pawns.states[i] = PawnState::Eating;
        } else if (type == BuildingType::Residential &&
                   pawns.homes[i] == arrivedAt) {
          pawns.states[i] = PawnState::Sleeping;
        } else {
          pawns.states[i] = PawnState::Idle;
        }

        pawns.targetBuilding[i] = NULL_BUILDING;
      } else {
        // Arrived but no target building - go idle
        pawns.states[i] = PawnState::Idle;
      }
      continue;
    }

    // Move toward destination
    glm::vec3 dir = diff / dist;
    f32 moveAmount = pawns.speeds[i] * dt;

    // Don't overshoot
    if (moveAmount > dist) {
      moveAmount = dist;
    }

    glm::vec3 newPos = pos + dir * moveAmount;

    // Resolve building collisions (simple push-out)
    newPos = resolveCollisions(newPos, buildings);

    pawns.positions[i] = newPos;
  }
}

}  // namespace uinta
