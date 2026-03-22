#include "uinta/scenes/city_game/intersection_init.h"

#include <absl/random/random.h>

#include "uinta/scenes/city_game/building_types.h"

namespace uinta {

// Calculate entry point for a building (street-facing side)
glm::vec3 calculateEntryPoint(glm::vec3 position, glm::vec3 dimensions) {
  // Entry point is toward the center (0,0,0) which is where the streets are
  // Place entry point just outside the building on the street-facing side
  glm::vec3 halfDims = dimensions * 0.5f;

  f32 entryX = position.x;
  f32 entryZ = position.z;

  // Move entry point from building center toward the street (origin)
  // The entry should be outside the building footprint
  if (position.x < 0) {
    // Building is on the left side, entry on right edge (toward center)
    entryX = position.x + halfDims.x + 1.0f;
  } else {
    // Building is on the right side, entry on left edge (toward center)
    entryX = position.x - halfDims.x - 1.0f;
  }

  if (position.z < 0) {
    // Building is on the back side, entry on front edge (toward center)
    entryZ = position.z + halfDims.z + 1.0f;
  } else {
    // Building is on the front side, entry on back edge (toward center)
    entryZ = position.z - halfDims.z - 1.0f;
  }

  return glm::vec3(entryX, 0.0f, entryZ);
}

void initIntersectionBuildings(IntersectionBuildings& buildings) noexcept {
  // Intersection layout (4 quadrants around center at 0,0):
  //
  //      (-12, -12)  |  (12, -12)
  //      Residential |  Workplace
  //     -------------+-------------
  //         Shop     |  Residential
  //      (-12, 12)   |  (12, 12)
  //
  // Streets run through the center (approximately -2 to 2 on both axes)

  // Building dimensions
  constexpr glm::vec3 residentialDims(8.0f, 0.3f, 8.0f);
  constexpr glm::vec3 workplaceDims(10.0f, 0.5f, 10.0f);
  constexpr glm::vec3 shopDims(6.0f, 0.4f, 6.0f);

  // Quadrant centers (offset from origin to leave room for streets)
  constexpr f32 offset = 10.0f;

  // Residential A (NW quadrant)
  {
    glm::vec3 pos(-offset, 0, -offset);
    glm::vec3 entry = calculateEntryPoint(pos, residentialDims);
    buildings.add(pos, residentialDims, entry, BuildingType::Residential, 6);
  }

  // Workplace (NE quadrant)
  {
    glm::vec3 pos(offset, 0, -offset);
    glm::vec3 entry = calculateEntryPoint(pos, workplaceDims);
    buildings.add(pos, workplaceDims, entry, BuildingType::Workplace, 20);
  }

  // Shop (SW quadrant)
  {
    glm::vec3 pos(-offset, 0, offset);
    glm::vec3 entry = calculateEntryPoint(pos, shopDims);
    buildings.add(pos, shopDims, entry, BuildingType::Shop,
                  255);  // No real limit for shops
  }

  // Residential B (SE quadrant)
  {
    glm::vec3 pos(offset, 0, offset);
    glm::vec3 entry = calculateEntryPoint(pos, residentialDims);
    buildings.add(pos, residentialDims, entry, BuildingType::Residential, 6);
  }
}

void initPawns(Pawns& pawns, const IntersectionBuildings& buildings,
               const PawnConfig& config, size_t count) noexcept {
  static auto bitgen = absl::BitGen();

  // Find building IDs
  BuildingId residentialA = NULL_BUILDING;
  BuildingId residentialB = NULL_BUILDING;
  BuildingId workplace = NULL_BUILDING;

  for (size_t i = 0; i < buildings.count(); ++i) {
    if (buildings.types[i] == BuildingType::Residential) {
      if (residentialA == NULL_BUILDING) {
        residentialA = buildings.ids[i];
      } else {
        residentialB = buildings.ids[i];
      }
    } else if (buildings.types[i] == BuildingType::Workplace) {
      workplace = buildings.ids[i];
    }
  }

  // Create pawns distributed between the two residential buildings
  for (size_t i = 0; i < count; ++i) {
    // Alternate homes between the two residential buildings
    BuildingId home = (i % 2 == 0) ? residentialA : residentialB;

    // All pawns work at the workplace
    BuildingId job = workplace;

    // Start position at home entry point
    glm::vec3 startPos = buildings.entryPoints[home];

    // Varied initial conditions
    f32 initialHunger = absl::Uniform(bitgen, 0.0f, 0.3f);
    f32 initialEnergy = absl::Uniform(bitgen, 0.7f, 1.0f);
    f32 initialMoney = absl::Uniform(bitgen, 50.0f, 200.0f);

    pawns.add(startPos, config.walkingSpeed, initialHunger, initialEnergy,
              initialMoney, home, job);
  }
}

}  // namespace uinta
