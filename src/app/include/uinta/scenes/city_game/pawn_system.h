#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_SYSTEM_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_SYSTEM_H_

#include "./intersection_data.h"
#include "./pawn_data.h"
#include "./pawn_types.h"
#include "./sim_time.h"

namespace uinta {

// Update pawn needs (hunger, energy) based on current state
void updatePawnNeeds(Pawns& pawns, const PawnConfig& config,
                     f32 simHours) noexcept;

// Determine the next state for a pawn based on needs and time
PawnState decidePawnState(const Pawns& pawns,
                          const IntersectionBuildings& buildings,
                          const SimTime& time, const PawnConfig& config,
                          size_t index) noexcept;

// Handle state transitions (enter/exit states, set destinations)
void transitionPawnState(Pawns& pawns, IntersectionBuildings& buildings,
                         const SimTime& time, const PawnConfig& config,
                         size_t index, PawnState newState) noexcept;

// Main decision system - runs every tick
void pawnDecisionSystem(Pawns& pawns, IntersectionBuildings& buildings,
                        const SimTime& time, const PawnConfig& config,
                        f32 dt) noexcept;

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_SYSTEM_H_
