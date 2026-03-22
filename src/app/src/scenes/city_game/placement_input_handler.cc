#include "uinta/scenes/city_game/placement_input_handler.h"

#include <glm/common.hpp>

#include "uinta/engine/engine.h"
#include "uinta/input/input_system.h"
#include "uinta/input/input_token.h"
#include "uinta/scene/scene.h"

namespace uinta {

PlacementInputHandler::PlacementInputHandler(Scene* scene) noexcept {
  auto* engine = scene->engine();
  input_ = engine->input();
  auto* inputSystem = scene->findComponent<InputSystem>().value();

  // Key 1: House building mode
  inputSystem->subscribeKey(Key::Num1 | Action::Press, [&](Key, Action, Mod) {
    mode_ = PlacementMode::Building;
    activeBuildingType_ = BuildingType::House;
  });

  // Key 2: Factory building mode
  inputSystem->subscribeKey(Key::Num2 | Action::Press, [&](Key, Action, Mod) {
    mode_ = PlacementMode::Building;
    activeBuildingType_ = BuildingType::Factory;
  });

  // Key 3: Road placement mode
  inputSystem->subscribeKey(Key::Num3 | Action::Press, [&](Key, Action, Mod) {
    mode_ = PlacementMode::Road;
  });

  // Mouse click: placement request
  inputSystem->subscribeMouse(MouseBtn::Left | Action::Release,
                              [&](auto, auto, auto) {
                                auto worldPos = input_->cursorWorldPoint();
                                placementPosition_ = worldPos.xz();
                                hasPlacementRequest_ = true;
                              });
}

}  // namespace uinta
