#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_DATA_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_DATA_H_

#include <glm/ext/vector_float3.hpp>
#include <vector>

#include "./pawn_types.h"
#include "uinta/types.h"

namespace uinta {

using BuildingId = u32;
constexpr BuildingId NULL_BUILDING = ~0u;

// Structure-of-Arrays layout for cache efficiency
struct Pawns {
  // Identity
  std::vector<PawnId> ids;

  // Spatial
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> destinations;
  std::vector<f32> speeds;  // units per second

  // Needs (0.0 - 1.0 range)
  std::vector<f32> hunger;  // 0 = full, 1 = starving
  std::vector<f32> energy;  // 0 = exhausted, 1 = fully rested

  // Resources
  std::vector<f32> money;

  // State
  std::vector<PawnState> states;
  std::vector<BuildingId> currentBuilding;  // NULL_BUILDING if outdoors
  std::vector<BuildingId>
      targetBuilding;  // Where pawn is heading (for Walking state)

  // Assignments
  std::vector<BuildingId> homes;  // where they sleep
  std::vector<BuildingId> jobs;   // where they work

  // Helpers
  size_t count() const noexcept { return ids.size(); }
  bool empty() const noexcept { return ids.empty(); }

  // Add a new pawn, returns the PawnId
  PawnId add(glm::vec3 position, f32 speed, f32 initialHunger,
             f32 initialEnergy, f32 initialMoney, BuildingId home,
             BuildingId job) noexcept {
    PawnId id = static_cast<PawnId>(ids.size());
    ids.push_back(id);
    positions.push_back(position);
    destinations.push_back(position);
    speeds.push_back(speed);
    hunger.push_back(initialHunger);
    energy.push_back(initialEnergy);
    money.push_back(initialMoney);
    states.push_back(PawnState::Idle);
    currentBuilding.push_back(home);  // Start inside home
    targetBuilding.push_back(NULL_BUILDING);
    homes.push_back(home);
    jobs.push_back(job);
    return id;
  }
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_DATA_H_
