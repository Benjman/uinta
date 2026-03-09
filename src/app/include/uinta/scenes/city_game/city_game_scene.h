#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_

#include "./placement_input_handler.h"
#include "./building_system.h"
#include "./building_types.h"
#include "./city_renderer.h"
#include "./road_system.h"
#include "./road_renderer.h"
#include "./economy_system.h"
#include "uinta/scene/scene.h"

namespace uinta {

class CityGameScene : public Scene {
 public:
  explicit CityGameScene(Scene* parent) noexcept;

  void postTick(time_t dt) noexcept override;
  void render(time_t dt) noexcept override;
  void onDebugUi() noexcept override;

 private:
  BuildingSystem buildingSystem_;
  RoadSystem roadSystem_;
  EconomySystem economySystem_;
  CityRenderer cityRenderer_;
  RoadRenderer roadRenderer_;
  PlacementInputHandler inputHandler_;

  bool tryPlaceBuilding(glm::vec2 position, BuildingType type) noexcept;
  bool tryPlaceRoad(glm::ivec2 position) noexcept;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_
