#include "uinta/camera/camera_manager.h"

#include <cassert>
#include <glm/geometric.hpp>

#include "uinta/app_config.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_serializer.h"
#include "uinta/engine/engine.h"
#include "uinta/input/input_system.h"
#include "uinta/math/direction.h"
#include "uinta/math/spatial.h"

namespace uinta {

CameraManager::CameraManager(Engine* engine) noexcept
    : serviceHandle_(engine, this),
      camera_(deserialize(engine->service<AppConfig>())),
      keybindings_(deserializeKeybindings(engine->service<AppConfig>())),
      appConfig_(engine->service<AppConfig>()),
      inputSystem_(engine->service<InputSystem>()) {
  assert(inputSystem_ && "CameraManager: InputSystem must be registered as a service!");
  subscribe();
}

CameraManager::~CameraManager() noexcept {
  for (auto handle : subscriptions_) {
    inputSystem_->unsubscribe(handle);
  }
  serialize(appConfig_, camera_.config());
  serializeKeybindings(appConfig_, keybindings_);
}

void CameraManager::subscribe() noexcept {
  const auto& kb = keybindings_;
  for (auto key : {kb.anglNegK, kb.anglPosK, kb.dstDecK, kb.dstIncK, kb.pitchNegK, kb.pitchPosK, kb.backwardK,
                   kb.forwardK, kb.leftK, kb.rightK}) {
    subscribeKey(key);
  }
  for (auto button : {kb.angleM, kb.pitchM, kb.trnslM}) {
    subscribeMouse(button);
  }

  subscriptions_.push_back(inputSystem_->subscribeMouseMove([this](const MouseMoveInfo& info) {
    cursordx_ += info.dx;
    cursordy_ += info.dy;
  }));
  subscriptions_.push_back(
      inputSystem_->subscribeMouseScroll([this](const MouseScrollInfo& info) { scrolldy_ += info.dy; }));
}

// InputSystem matches modifiers exactly, so held state is tracked by subscribing to every combination of
// Shift/Control/Alt/Super. Otherwise a key released while a modifier is held would remain down.
constexpr u32 ModCombinations = 16;

void CameraManager::subscribeKey(Key key) noexcept {
  if (key == Key::Unspecified) {
    return;
  }
  for (u32 i = 0; i < ModCombinations; i++) {
    auto token = (key | Action::Press) | static_cast<u32>(Action::Release) | (i * static_cast<u32>(Mod::Shift));
    subscriptions_.push_back(inputSystem_->subscribeKey(token, [this](const KeyInfo& info) {
      keysDown_.set(static_cast<size_t>(info.key), info.action == Action::Press);
    }));
  }
}

void CameraManager::subscribeMouse(MouseBtn button) noexcept {
  if (button == MouseBtn::Unspecified) {
    return;
  }
  for (u32 i = 0; i < ModCombinations; i++) {
    auto token = (button | Action::Press) | static_cast<u32>(Action::Release) | (i * static_cast<u32>(Mod::Shift));
    subscriptions_.push_back(inputSystem_->subscribeMouse(token, [this](const MouseButtonInfo& info) {
      mouseDown_.set(static_cast<size_t>(info.button), info.action == Action::Press);
    }));
  }
}

void CameraManager::update(time_t delta) noexcept {
  auto deltas = computeDeltas(delta);
  cursordx_ = 0;
  cursordy_ = 0;
  scrolldy_ = 0;
  camera_.update(delta, deltas);
  if (comparator_ == &camera_) {
    return;
  }
  comparator_ = camera_;
  dispatchers_.dispatch<CameraEvent::ViewMatrixUpdated>(ViewMatrixUpdateEvent(&camera_, camera_.viewMatrix()));
}

CameraInputDeltas CameraManager::computeDeltas(time_t delta) const noexcept {
  CameraInputDeltas deltas;
  const auto& config = camera_.config();
  const auto& kb = keybindings_;

  // Angle delta
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.anglNegK)) {
    deltas.angle += kb.anglSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.anglPosK)) {
    deltas.angle -= kb.anglSpdK;
  }
  if (config.flags.isMouseEnabled() && isMouseDown(kb.angleM) && (cursordx_ != 0.0f)) {
    deltas.angle += cursordx_ * kb.anglSpdM;
  }
  deltas.angle *= static_cast<f32>(delta);

  // Pitch delta
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.pitchPosK)) {
    deltas.pitch += kb.pitchSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.pitchNegK)) {
    deltas.pitch -= kb.pitchSpdK;
  }
  if (config.flags.isMouseEnabled() && isMouseDown(kb.pitchM) && (cursordy_ != 0.0f)) {
    deltas.pitch += cursordy_ * kb.pitchSpdM;
  }
  deltas.pitch *= static_cast<f32>(delta);

  // Apply translation factor for distance and translation
  auto translationDelta = delta * camera_.translationFactor();
  deltas.translationDelta = translationDelta;

  // Distance delta
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.dstIncK)) {
    deltas.dist -= kb.dstSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.dstDecK)) {
    deltas.dist += kb.dstSpdK;
  }
  if (config.flags.isMouseEnabled() && scrolldy_ != 0.0f) {
    deltas.dist += scrolldy_ * -kb.dstSpdM;
  }
  deltas.dist *= static_cast<f32>(translationDelta);

  // Translation delta
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.forwardK)) {
    deltas.translation += glm::normalize(WorldHorizontal * getForward(config.pitch, config.angle)) * kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.backwardK)) {
    deltas.translation -= glm::normalize(WorldHorizontal * getForward(config.pitch, config.angle)) * kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.leftK)) {
    deltas.translation += -glm::normalize(getRight(config.angle) * WorldHorizontal) * kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && isKeyDown(kb.rightK)) {
    deltas.translation += glm::normalize(getRight(config.angle) * WorldHorizontal) * kb.trnslSpdK;
  }
  if (config.flags.isMouseEnabled() && isMouseDown(kb.trnslM) && ((cursordx_ != 0.0f) || (cursordy_ != 0.0f))) {
    deltas.translation -= (glm::normalize(WorldHorizontal * getRight(config.angle)) * cursordx_ -
                           glm::normalize(WorldHorizontal * getForward(config.pitch, config.angle)) * cursordy_) *
                          kb.trnslSpdM;
  }
  deltas.translation *= static_cast<f32>(translationDelta);

  return deltas;
}

}  // namespace uinta
