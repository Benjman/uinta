#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_INPUT_HANDLER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_INPUT_HANDLER_H_

#include "./building_types.h"

namespace uinta {

class Input;
class Scene;

class BuildingInputHandler {
 public:
  BuildingInputHandler(Scene*) noexcept;

  BuildingType activeBuildingType() const noexcept {
    return activeBuildingType_;
  }

  void activeBuildingType(BuildingType type) noexcept {
    activeBuildingType_ = type;
  }

  bool hasPlacementRequest() const noexcept { return hasPlacementRequest_; }
  glm::vec2 placementPosition() const noexcept { return placementPosition_; }
  void clearPlacementRequest() noexcept { hasPlacementRequest_ = false; }

 private:
  BuildingType activeBuildingType_ = BuildingType::House;
  bool hasPlacementRequest_ = false;
  glm::vec2 placementPosition_{0};
  Input* input_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_BUILDING_INPUT_HANDLER_H_
