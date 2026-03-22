#include "uinta/scenes/city_game/city_game_scene.h"

#include <imgui.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "uinta/engine/engine.h"
#include "uinta/scenes/city_game/intersection_init.h"
#include "uinta/scenes/city_game/movement_system.h"
#include "uinta/scenes/city_game/pawn_system.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

CityGameScene::CityGameScene(Scene* parent) noexcept
    : Scene(parent, SceneLayer::Simulation),
      simTime_{8.0f, 0, 1.0f},  // Start at 8 AM, day 0, 1x speed
      shaderManager_(findComponent<BasicShaderManager>().value_or(nullptr)),
      intersectionRenderer_(shaderManager_, engine()->gl()),
      pawnRenderer_(shaderManager_, engine()->gl()) {
  // Initialize the intersection layout
  initIntersectionBuildings(buildings_);

  // Initialize pawns (15 pawns, all employed)
  initPawns(pawns_, buildings_, pawnConfig_, 15);

  // Build rendering mesh for buildings
  intersectionRenderer_.buildMesh(buildings_);

  // Set initial lighting based on start time
  updateLighting();
}

void CityGameScene::postTick(time_t dt) noexcept {
  if (simulationPaused_) {
    return;
  }

  // Scale dt by time scale for simulation
  f32 scaledDt = static_cast<f32>(dt) * simTime_.timeScale;

  // Advance simulation time
  simTime_.advance(dt);

  // Update lighting based on time of day
  updateLighting();

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

glm::vec3 CityGameScene::calculateSunDirection(f32 hour) const noexcept {
  // Sun arc: rises at 6 AM (east), peaks at noon (overhead), sets at 18 (west)
  // Map hour to angle: 6:00 = 0 degrees (horizon east), 12:00 = 90 degrees
  // (zenith), 18:00 = 180 degrees (horizon west)

  constexpr f32 sunriseHour = 6.0f;
  constexpr f32 sunsetHour = 18.0f;
  constexpr f32 dayLength = sunsetHour - sunriseHour;  // 12 hours

  // Clamp hour to daytime range for sun position calculation
  f32 dayHour = glm::clamp(hour, sunriseHour, sunsetHour);

  // Normalize to 0-1 range (0 = sunrise, 1 = sunset)
  f32 dayProgress = (dayHour - sunriseHour) / dayLength;

  // Convert to angle: 0 = east horizon, PI/2 = overhead, PI = west horizon
  f32 sunAngle = dayProgress * glm::pi<f32>();

  // Calculate sun direction (pointing FROM the sun TO the scene)
  // X: east-west axis (positive = east)
  // Y: up-down axis (positive = up)
  // Z: north-south axis
  f32 x = std::cos(sunAngle);  // East at sunrise, west at sunset
  f32 y = std::sin(sunAngle);  // Peaks at noon
  f32 z = -0.3f;               // Slight tilt for more interesting shadows

  // During night hours, place sun below horizon
  if (hour < sunriseHour || hour > sunsetHour) {
    // Night time - sun is below horizon
    // Smoothly transition for dawn/dusk
    if (hour < sunriseHour) {
      f32 nightProgress = hour / sunriseHour;  // 0 at midnight, 1 at sunrise
      y = -0.5f + nightProgress * 0.5f;        // Rise from -0.5 to 0
      x = 1.0f;                                // Coming from east
    } else {
      f32 nightProgress =
          (hour - sunsetHour) / (24.0f - sunsetHour);  // 0 at sunset, 1 at
                                                       // midnight
      y = -nightProgress * 0.5f;                       // Descend from 0 to -0.5
      x = -1.0f;                                       // Going to west
    }
  }

  // Return normalized direction vector (pointing toward scene, so negate)
  return glm::normalize(glm::vec3(-x, -y, z));
}

void CityGameScene::updateLighting() noexcept {
  if (!shaderManager_) {
    return;
  }

  ShaderGuard sg(shaderManager_->shader());

  glm::vec3 sunDir = calculateSunDirection(simTime_.hour);
  shaderManager_->lightDir(sunDir);

  // Adjust ambient light based on time of day
  // Brighter during day, dimmer at night
  constexpr f32 sunriseHour = 6.0f;
  constexpr f32 sunsetHour = 18.0f;

  f32 ambient;
  if (simTime_.hour >= sunriseHour && simTime_.hour <= sunsetHour) {
    // Daytime: higher ambient (0.4 - 0.5)
    f32 dayProgress =
        (simTime_.hour - sunriseHour) / (sunsetHour - sunriseHour);
    // Peak at noon
    f32 noonFactor = 1.0f - std::abs(dayProgress - 0.5f) * 2.0f;
    ambient = 0.35f + noonFactor * 0.15f;
  } else {
    // Nighttime: lower ambient (0.15 - 0.25)
    ambient = 0.2f;
  }
  shaderManager_->ambientStr(ambient);

  // Adjust light color based on time of day
  glm::vec3 lightColor;
  if (simTime_.hour < 7.0f || simTime_.hour > 17.0f) {
    // Dawn/dusk: warm orange tones
    lightColor = glm::vec3(1.0f, 0.7f, 0.5f);
  } else if (simTime_.hour >= 7.0f && simTime_.hour <= 17.0f) {
    // Daytime: white/slightly warm
    lightColor = glm::vec3(1.0f, 0.95f, 0.9f);
  } else {
    // Night: cool blue moonlight
    lightColor = glm::vec3(0.6f, 0.7f, 0.9f);
  }

  // Transition smoothing for dawn (6-7) and dusk (17-18)
  if (simTime_.hour >= 6.0f && simTime_.hour < 7.0f) {
    f32 t = simTime_.hour - 6.0f;  // 0 to 1
    lightColor =
        glm::mix(glm::vec3(1.0f, 0.6f, 0.4f), glm::vec3(1.0f, 0.95f, 0.9f), t);
  } else if (simTime_.hour >= 17.0f && simTime_.hour < 18.0f) {
    f32 t = simTime_.hour - 17.0f;  // 0 to 1
    lightColor =
        glm::mix(glm::vec3(1.0f, 0.95f, 0.9f), glm::vec3(1.0f, 0.6f, 0.4f), t);
  }

  shaderManager_->lightColor(lightColor);
}

}  // namespace uinta
