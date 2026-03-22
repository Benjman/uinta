#include "uinta/scenes/city_game/building_system.h"

#include <ranges>

namespace uinta {

Building& BuildingSystem::addBuilding(glm::vec2 position,
                                      BuildingType type) noexcept {
  return buildings_.emplace_back(Building{
      .position = position,
      .type = type,
  });
}

size_t BuildingSystem::count(BuildingType type) const noexcept {
  return std::ranges::distance(buildings_ |
                               std::views::filter([type](const auto& building) {
                                 return building.type == type;
                               }));
}

}  // namespace uinta
