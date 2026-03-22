#include "uinta/scenes/city_game/city_game_scene.h"

#include <imgui.h>

#include "uinta/engine/engine.h"
#include "uinta/scenes/city_game/building_types.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

CityGameScene::CityGameScene(Scene* parent) noexcept
    : Scene(parent, SceneLayer::Simulation),
      economySystem_(initialBuildingCost(BuildingType::House) * 5),
      renderer_(findComponent<BasicShaderManager>().value_or(nullptr),
                engine()->gl()),
      inputHandler_(this) {
  // Place initial buildings
  economySystem_.setMoney(100000);
  for (i32 i = -5; i < 5; i++) {
    tryPlaceBuilding(glm::vec2(i, 0), BuildingType::House);
  }
  economySystem_.setMoney(initialBuildingCost(BuildingType::House) * 5);
}

void CityGameScene::postTick(time_t dt) noexcept {
  economySystem_.update(dt, buildingSystem_.houseCount(),
                        buildingSystem_.factoryCount());

  if (inputHandler_.hasPlacementRequest()) {
    tryPlaceBuilding(inputHandler_.placementPosition(),
                     inputHandler_.activeBuildingType());
    inputHandler_.clearPlacementRequest();
  }
}

void CityGameScene::render(time_t) noexcept { renderer_.render(); }

void CityGameScene::onDebugUi() noexcept {
  auto income = economySystem_.incomePerSecond(buildingSystem_.houseCount(),
                                               buildingSystem_.factoryCount());
  ImGui::Text("Money: %.1f (+%.1f/s)", economySystem_.money(), income);
  ImGui::Separator();

  ImGui::Text("Select Building (1-2):");
  auto activeType = inputHandler_.activeBuildingType();

  if (ImGui::RadioButton("[1] House", activeType == BuildingType::House)) {
    inputHandler_.activeBuildingType(BuildingType::House);
  }
  ImGui::SameLine();
  ImGui::Text("Cost: %.1f",
              economySystem_.buildingCost(BuildingType::House,
                                          buildingSystem_.houseCount()));

  if (ImGui::RadioButton("[2] Factory", activeType == BuildingType::Factory)) {
    inputHandler_.activeBuildingType(BuildingType::Factory);
  }
  ImGui::SameLine();
  ImGui::Text("Cost: %.1f",
              economySystem_.buildingCost(BuildingType::Factory,
                                          buildingSystem_.factoryCount()));

  ImGui::Separator();
  ImGui::Text("Houses: %zu (+%.1f/s)", buildingSystem_.houseCount(),
              economySystem_.houseMoney(buildingSystem_.houseCount()));
  ImGui::Text("Factories: %zu (+%.1f/s)", buildingSystem_.factoryCount(),
              economySystem_.factoryMoney(buildingSystem_.factoryCount()));
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
  renderer_.addBuilding(building);

  return true;
}

}  // namespace uinta
