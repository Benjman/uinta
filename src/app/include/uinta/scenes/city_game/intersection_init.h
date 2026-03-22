#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_INIT_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_INIT_H_

#include "./intersection_data.h"
#include "./pawn_data.h"
#include "./pawn_types.h"

namespace uinta {

// Initialize the 4-building intersection layout
void initIntersectionBuildings(IntersectionBuildings& buildings) noexcept;

// Initialize pawns with varied starting conditions
// All pawns will have jobs (no unemployed pawns)
void initPawns(Pawns& pawns, const IntersectionBuildings& buildings,
               const PawnConfig& config, size_t count = 15) noexcept;

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_INIT_H_
