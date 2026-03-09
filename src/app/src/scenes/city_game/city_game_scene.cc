#include "uinta/scenes/city_game/city_game_scene.h"

#include <imgui.h>

#include "uinta/engine/engine.h"
#include "uinta/scenes/city_game/building_types.h"
#include "uinta/scenes/city_game/road_types.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

CityGameScene::CityGameScene(Scene* parent) noexcept
    : Scene(parent, SceneLayer::Simulation),
      economySystem_(initialBuildingCost(BuildingType::House) * 5),
      cityRenderer_(findComponent<BasicShaderManager>().value_or(nullptr),
                    engine()->gl()),
      roadRenderer_(findComponent<BasicShaderManager>().value_or(nullptr),
                    engine()->gl()),
      inputHandler_(this) {
  // Place initial buildings
  constexpr size_t InitialBuildingCount = 10;
  for (size_t i = 0; i < InitialBuildingCount; i++) {
    auto position = glm::vec2(-5 + static_cast<i32>(i), 0);
    cityRenderer_.addBuilding(
        buildingSystem_.addBuilding(position, BuildingType::House));
  }
}

void CityGameScene::postTick(time_t dt) noexcept {
  economySystem_.update(dt, buildingSystem_.houseCount(),
                        buildingSystem_.factoryCount());

  if (inputHandler_.hasPlacementRequest()) {
    // Handle placement based on mode
    if (inputHandler_.mode() == PlacementMode::Building) {
      tryPlaceBuilding(inputHandler_.placementPosition(),
                       inputHandler_.activeBuildingType());
    } else {  // PlacementMode::Road
      tryPlaceRoad(inputHandler_.placementPosition());
    }
    inputHandler_.clearPlacementRequest();
  }
}

void CityGameScene::render(time_t) noexcept {
  roadRenderer_.render();
  cityRenderer_.render();
}

void CityGameScene::onDebugUi() noexcept {
  auto income = economySystem_.incomePerSecond(buildingSystem_.houseCount(),
                                               buildingSystem_.factoryCount());
  ImGui::Text("Money: %.1f (+%.1f/s)", economySystem_.money(), income);
  ImGui::Separator();

  ImGui::Text("Select Placement (1-3):");
  auto mode = inputHandler_.mode();
  auto activeType = inputHandler_.activeBuildingType();

  // Building options
  if (ImGui::RadioButton("[1] House", mode == PlacementMode::Building &&
                                          activeType == BuildingType::House)) {
    inputHandler_.mode(PlacementMode::Building);
    inputHandler_.activeBuildingType(BuildingType::House);
  }
  ImGui::SameLine();
  ImGui::Text("Cost: %.1f",
              economySystem_.buildingCost(BuildingType::House,
                                          buildingSystem_.houseCount()));

  if (ImGui::RadioButton("[2] Factory",
                         mode == PlacementMode::Building &&
                             activeType == BuildingType::Factory)) {
    inputHandler_.mode(PlacementMode::Building);
    inputHandler_.activeBuildingType(BuildingType::Factory);
  }
  ImGui::SameLine();
  ImGui::Text("Cost: %.1f",
              economySystem_.buildingCost(BuildingType::Factory,
                                          buildingSystem_.factoryCount()));

  // Road option
  if (ImGui::RadioButton("[3] Road", mode == PlacementMode::Road)) {
    inputHandler_.mode(PlacementMode::Road);
  }
  ImGui::SameLine();
  ImGui::Text("Cost: %.1f", economySystem_.roadCost(roadSystem_.count()));

  ImGui::Separator();
  ImGui::Text("Houses: %zu (+%.1f/s)", buildingSystem_.houseCount(),
              economySystem_.houseMoney(buildingSystem_.houseCount()));
  ImGui::Text("Factories: %zu (+%.1f/s)", buildingSystem_.factoryCount(),
              economySystem_.factoryMoney(buildingSystem_.factoryCount()));
  ImGui::Text("Roads: %zu", roadSystem_.count());
}

bool CityGameScene::tryPlaceBuilding(glm::vec2 position,
                                     BuildingType type) noexcept {
  auto cost = economySystem_.buildingCost(type, buildingSystem_.count(type));

  if (!economySystem_.canAfford(cost)) {
    return false;
  }

  economySystem_.deductMoney(cost);
  buildingSystem_.addBuilding(position, type);

  auto& building = buildingSystem_.buildings().back();
  cityRenderer_.addBuilding(building);

  return true;
}

bool CityGameScene::tryPlaceRoad(glm::ivec2 position) noexcept {
  // Check if road already exists at position
  if (roadSystem_.hasRoad(position)) {
    return false;
  }

  auto cost = economySystem_.roadCost(roadSystem_.count());

  if (!economySystem_.canAfford(cost)) {
    return false;
  }

  economySystem_.deductMoney(cost);
  roadSystem_.addRoad(position);

  // Render new road and all updated neighbors
  auto updatedPositions = roadSystem_.getLastUpdatedRoads();
  for (const auto& pos : updatedPositions) {
    if (auto* road = roadSystem_.getRoad(pos)) {
      roadRenderer_.addRoad(*road);
    }
  }

  return true;
}

}  // namespace uinta
