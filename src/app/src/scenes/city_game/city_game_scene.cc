#include "uinta/scenes/city_game/city_game_scene.h"

#include <imgui.h>

#include "uinta/engine/engine.h"
#include "uinta/scenes/city_game/intersection_init.h"
#include "uinta/scenes/city_game/movement_system.h"
#include "uinta/scenes/city_game/pawn_system.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

CityGameScene::CityGameScene(Scene* parent) noexcept
    : Scene(parent, SceneLayer::Simulation),
      simTime_{8.0f, 0, 1.0f},  // Start at 8 AM, day 0, 1x speed
      intersectionRenderer_(
          findComponent<BasicShaderManager>().value_or(nullptr),
          engine()->gl()),
      pawnRenderer_(findComponent<BasicShaderManager>().value_or(nullptr),
                    engine()->gl()) {
  // Initialize the intersection layout
  initIntersectionBuildings(buildings_);

  // Initialize pawns (15 pawns, all employed)
  initPawns(pawns_, buildings_, pawnConfig_, 15);

  // Build rendering mesh for buildings
  intersectionRenderer_.buildMesh(buildings_);
}

void CityGameScene::postTick(time_t dt) noexcept {
  if (simulationPaused_) {
    return;
  }

  // Scale dt by time scale for simulation
  f32 scaledDt = static_cast<f32>(dt) * simTime_.timeScale;

  // Advance simulation time
  simTime_.advance(dt);

  // Run pawn AI decisions and need updates
  pawnDecisionSystem(pawns_, buildings_, simTime_, pawnConfig_, scaledDt);

  // Move walking pawns
  movementSystem(pawns_, buildings_, scaledDt);
}

void CityGameScene::render(time_t) noexcept {
  // Render intersection (ground + buildings)
  intersectionRenderer_.render();

  // Render pawns
  pawnRenderer_.render(pawns_);
}

void CityGameScene::onDebugUi() noexcept {
  ImGui::Text("Living Intersection Prototype");
  ImGui::Separator();

  // Time display
  ImGui::Text("Day %d, %02d:%02d", simTime_.day, simTime_.hourInt(),
              simTime_.minuteInt());

  // Time scale slider
  ImGui::SliderFloat("Time Scale", &simTime_.timeScale, 0.1f, 60.0f, "%.1fx");
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("1x = 1 sim hour per real minute");
  }

  // Pause control
  ImGui::Checkbox("Pause Simulation", &simulationPaused_);

  ImGui::Separator();

  // Pawn statistics
  ImGui::Text("Pawns: %zu", pawns_.count());

  // Count by state
  int stateCounts[5] = {0, 0, 0, 0, 0};
  for (size_t i = 0; i < pawns_.count(); ++i) {
    stateCounts[static_cast<int>(pawns_.states[i])]++;
  }

  ImGui::Text("  Idle: %d", stateCounts[0]);
  ImGui::Text("  Walking: %d", stateCounts[1]);
  ImGui::Text("  Working: %d", stateCounts[2]);
  ImGui::Text("  Eating: %d", stateCounts[3]);
  ImGui::Text("  Sleeping: %d", stateCounts[4]);

  ImGui::Separator();

  // Show individual pawn info (collapsible)
  if (ImGui::CollapsingHeader("Pawn Details")) {
    for (size_t i = 0; i < pawns_.count(); ++i) {
      ImGui::PushID(static_cast<int>(i));

      // Only show pawns that are visible (outdoors)
      const char* location =
          pawns_.currentBuilding[i] == NULL_BUILDING ? "Outside" : "Inside";

      ImGui::Text("Pawn %zu [%s] - %s", i, pawnStateName(pawns_.states[i]),
                  location);
      ImGui::Text("  Hunger: %.0f%%  Energy: %.0f%%  Money: $%.0f",
                  pawns_.hunger[i] * 100.0f, pawns_.energy[i] * 100.0f,
                  pawns_.money[i]);

      ImGui::PopID();
    }
  }

  ImGui::Separator();

  // Building info
  if (ImGui::CollapsingHeader("Buildings")) {
    for (size_t i = 0; i < buildings_.count(); ++i) {
      const char* typeName = "Unknown";
      switch (buildings_.types[i]) {
        case BuildingType::Residential:
          typeName = "Residential";
          break;
        case BuildingType::Workplace:
          typeName = "Workplace";
          break;
        case BuildingType::Shop:
          typeName = "Shop";
          break;
        default:
          break;
      }
      ImGui::Text("%s: %d/%d occupants", typeName,
                  buildings_.currentOccupants[i], buildings_.maxOccupants[i]);
    }
  }
}

}  // namespace uinta
