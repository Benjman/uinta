#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_

#include "./intersection_data.h"
#include "./intersection_renderer.h"
#include "./pawn_data.h"
#include "./pawn_renderer.h"
#include "./pawn_types.h"
#include "./sim_time.h"
#include "uinta/scene/scene.h"

namespace uinta {

class BasicShaderManager;

class CityGameScene : public Scene {
 public:
  explicit CityGameScene(Scene* parent) noexcept;

  void postTick(time_t dt) noexcept override;
  void render(time_t dt) noexcept override;
  void onDebugUi() noexcept override;

 private:
  // Calculate sun direction based on time of day
  glm::vec3 calculateSunDirection(f32 hour) const noexcept;

  // Update shader lighting based on current simulation time
  void updateLighting() noexcept;

  // Simulation data
  IntersectionBuildings buildings_;
  Pawns pawns_;
  SimTime simTime_;
  PawnConfig pawnConfig_;

  // Rendering
  BasicShaderManager* shaderManager_ = nullptr;
  IntersectionRenderer intersectionRenderer_;
  PawnRenderer pawnRenderer_;

  // Simulation control
  bool simulationPaused_ = false;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_CITY_GAME_SCENE_SCENE_H_
