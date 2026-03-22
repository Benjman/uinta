#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_SYSTEM_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_SYSTEM_H_

#include <vector>

#include "./building_types.h"

namespace uinta {

class BuildingSystem {
 public:
  BuildingSystem() = default;

  const std::vector<Building>& buildings() const noexcept { return buildings_; }
  bool empty() const noexcept { return buildings_.empty(); }

  Building& addBuilding(glm::vec2 position, BuildingType type) noexcept;

  size_t count(BuildingType type) const noexcept;
  size_t houseCount() const noexcept { return count(BuildingType::House); }
  size_t factoryCount() const noexcept { return count(BuildingType::Factory); }

 private:
  std::vector<Building> buildings_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_SYSTEM_H_
