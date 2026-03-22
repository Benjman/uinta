#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PLACEMENT_INPUT_HANDLER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PLACEMENT_INPUT_HANDLER_H_

#include <glm/ext/vector_int2.hpp>

#include "./building_types.h"

namespace uinta {

class Input;
class Scene;

enum class PlacementMode {
  Building,
  Road,
};

class PlacementInputHandler {
 public:
  PlacementInputHandler(Scene*) noexcept;

  // Mode management
  PlacementMode mode() const noexcept { return mode_; }
  void mode(PlacementMode m) noexcept { mode_ = m; }

  // Building mode state
  BuildingType activeBuildingType() const noexcept {
    return activeBuildingType_;
  }
  void activeBuildingType(BuildingType type) noexcept {
    activeBuildingType_ = type;
  }

  // Placement request state
  bool hasPlacementRequest() const noexcept { return hasPlacementRequest_; }

  glm::vec2 placementPosition() const noexcept { return placementPosition_; }

  void clearPlacementRequest() noexcept { hasPlacementRequest_ = false; }

 private:
  PlacementMode mode_ = PlacementMode::Road;
  BuildingType activeBuildingType_ = BuildingType::House;
  bool hasPlacementRequest_ = false;

  glm::vec2 placementPosition_{0};

  Input* input_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PLACEMENT_INPUT_HANDLER_H_
