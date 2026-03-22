#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_DATA_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_DATA_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <limits>
#include <vector>

#include "./building_types.h"
#include "./pawn_data.h"
#include "uinta/types.h"

namespace uinta {

// Structure-of-Arrays layout for intersection buildings
struct IntersectionBuildings {
  // Identity
  std::vector<BuildingId> ids;

  // Spatial
  std::vector<glm::vec3> positions;    // center of building base
  std::vector<glm::vec3> dimensions;   // width (x), height (y), depth (z)
  std::vector<glm::vec3> entryPoints;  // pathfinding destination

  // Type and function
  std::vector<BuildingType> types;

  // Occupancy
  std::vector<u8> maxOccupants;
  std::vector<u8> currentOccupants;

  // Helpers
  size_t count() const noexcept { return ids.size(); }
  bool empty() const noexcept { return ids.empty(); }

  // Add a new building, returns the BuildingId
  BuildingId add(glm::vec3 position, glm::vec3 dims, glm::vec3 entry,
                 BuildingType type, u8 maxOccupancy) noexcept {
    BuildingId id = static_cast<BuildingId>(ids.size());
    ids.push_back(id);
    positions.push_back(position);
    dimensions.push_back(dims);
    entryPoints.push_back(entry);
    types.push_back(type);
    maxOccupants.push_back(maxOccupancy);
    currentOccupants.push_back(0);
    return id;
  }

  // Find the first building of a given type
  BuildingId findByType(BuildingType type) const noexcept {
    for (size_t i = 0; i < types.size(); ++i) {
      if (types[i] == type) {
        return ids[i];
      }
    }
    return NULL_BUILDING;
  }

  // Find nearest building of a given type to a position
  BuildingId findNearestByType(BuildingType type,
                               glm::vec3 position) const noexcept {
    BuildingId nearest = NULL_BUILDING;
    f32 nearestDist = std::numeric_limits<f32>::max();

    for (size_t i = 0; i < types.size(); ++i) {
      if (types[i] == type) {
        f32 dist = glm::length(entryPoints[i] - position);
        if (dist < nearestDist) {
          nearestDist = dist;
          nearest = ids[i];
        }
      }
    }
    return nearest;
  }
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_DATA_H_
