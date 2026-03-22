#include "uinta/scenes/city_game/building_input_handler.h"

#include "uinta/engine/engine.h"
#include "uinta/input/input_system.h"
#include "uinta/input/input_token.h"
#include "uinta/scene/scene.h"

namespace uinta {

BuildingInputHandler::BuildingInputHandler(Scene* scene) noexcept {
  auto* engine = scene->engine();
  input_ = engine->input();
  auto* inputSystem = scene->findComponent<InputSystem>().value();
  inputSystem->subscribeKey(Key::Num1 | Action::Press, [&](Key, Action, Mod) {
    activeBuildingType_ = BuildingType::House;
  });
  inputSystem->subscribeKey(Key::Num2 | Action::Press, [&](Key, Action, Mod) {
    activeBuildingType_ = BuildingType::Factory;
  });
  inputSystem->subscribeMouse(MouseBtn::Left | Action::Release,
                              [&](auto, auto, auto) {
                                auto worldPos = input_->cursorWorldPoint();
                                placementPosition_ = worldPos.xz();
                                hasPlacementRequest_ = true;
                              });
}

}  // namespace uinta
