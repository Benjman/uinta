#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_MOVEMENT_SYSTEM_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_MOVEMENT_SYSTEM_H_

#include "./intersection_data.h"
#include "./pawn_data.h"

namespace uinta {

// Move walking pawns toward their destinations
void movementSystem(Pawns& pawns, IntersectionBuildings& buildings,
                    f32 dt) noexcept;

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_MOVEMENT_SYSTEM_H_
