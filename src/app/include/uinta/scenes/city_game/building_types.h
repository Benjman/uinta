#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_TYPES_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_TYPES_H_

#include <glm/ext/vector_float2.hpp>

#include "uinta/color.h"
#include "uinta/types.h"

namespace uinta {

enum class BuildingType {
  House,
  Factory,
  Residential,  // Living Intersection: where pawns sleep
  Workplace,    // Living Intersection: where pawns work
  Shop,         // Living Intersection: where pawns eat
};

inline f32 buildingTypeSize(BuildingType type) {
  switch (type) {
    case BuildingType::House:
      return 1;
    case BuildingType::Factory:
      return 3;
    case BuildingType::Residential:
      return 8;  // Larger buildings for intersection
    case BuildingType::Workplace:
      return 10;
    case BuildingType::Shop:
      return 6;
  }
  return 0;
}

inline Color buildingTypeColor(BuildingType type) {
  switch (type) {
    case BuildingType::House:
      return color::Green500;
    case BuildingType::Factory:
      return color::Orange500;
    case BuildingType::Residential:
      return color::LightBlue400;
    case BuildingType::Workplace:
      return color::Gray500;
    case BuildingType::Shop:
      return color::Amber500;
  }
  return color::Gray50;
}

inline i32 initialBuildingCost(BuildingType type) {
  switch (type) {
    case BuildingType::House:
      return 10;
    case BuildingType::Factory:
      return 50;
    case BuildingType::Residential:
      return 0;  // Not player-placeable
    case BuildingType::Workplace:
      return 0;
    case BuildingType::Shop:
      return 0;
  }
  return 0;
}

inline f32 buildingInflationRate(BuildingType) { return 1.05f; }

struct Building {
  glm::vec2 position;
  BuildingType type;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_TYPES_H_
