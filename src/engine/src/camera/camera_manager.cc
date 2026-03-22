#include "uinta/camera/camera_manager.h"

#include <glm/geometric.hpp>

#include "uinta/app_config.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_serializer.h"
#include "uinta/engine/engine.h"
#include "uinta/input.h"
#include "uinta/math/direction.h"
#include "uinta/math/spatial.h"

namespace uinta {

CameraManager::CameraManager(Engine* engine) noexcept
    : serviceHandle_(engine, this),
      camera_(deserialize(engine->service<AppConfig>())),
      keybindings_(deserializeKeybindings(engine->service<AppConfig>())),
      appConfig_(engine->service<AppConfig>()),
      input_(engine->input()) {}

CameraManager::~CameraManager() noexcept {
  serialize(appConfig_, camera_.config());
  serializeKeybindings(appConfig_, keybindings_);
}

void CameraManager::update(time_t delta) noexcept {
  auto deltas = computeDeltas(delta);
  camera_.update(delta, deltas);
  if (comparator_ == &camera_) {
    return;
  }
  comparator_ = camera_;
  dispatchers_.dispatch<CameraEvent::ViewMatrixUpdated>(
      ViewMatrixUpdateEvent(&camera_, camera_.viewMatrix()));
}

CameraInputDeltas CameraManager::computeDeltas(time_t delta) const noexcept {
  CameraInputDeltas deltas;
  const auto& config = camera_.config();
  const auto& kb = keybindings_;

  // Angle delta
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.anglNegK)) {
    deltas.angle += kb.anglSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.anglPosK)) {
    deltas.angle -= kb.anglSpdK;
  }
  if (config.flags.isMouseEnabled() && input_->isMouseDown(kb.angleM) &&
      (input_->cursordx() != 0.0f)) {
    deltas.angle += input_->cursordx() * kb.anglSpdM;
  }
  deltas.angle *= static_cast<f32>(delta);

  // Pitch delta
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.pitchPosK)) {
    deltas.pitch += kb.pitchSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.pitchNegK)) {
    deltas.pitch -= kb.pitchSpdK;
  }
  if (config.flags.isMouseEnabled() && input_->isMouseDown(kb.pitchM) &&
      (input_->cursordy() != 0.0f)) {
    deltas.pitch += input_->cursordy() * kb.pitchSpdM;
  }
  deltas.pitch *= static_cast<f32>(delta);

  // Apply translation factor for distance and translation
  auto translationDelta = delta * camera_.translationFactor();
  deltas.translationDelta = translationDelta;

  // Distance delta
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.dstIncK)) {
    deltas.dist -= kb.dstSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.dstDecK)) {
    deltas.dist += kb.dstSpdK;
  }
  if (config.flags.isMouseEnabled() && input_->mouseScrolled()) {
    deltas.dist += input_->scrolldy() * -kb.dstSpdM;
  }
  deltas.dist *= static_cast<f32>(translationDelta);

  // Translation delta
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.forwardK)) {
    deltas.translation +=
        glm::normalize(WorldHorizontal *
                       getForward(config.pitch, config.angle)) *
        kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.backwardK)) {
    deltas.translation -=
        glm::normalize(WorldHorizontal *
                       getForward(config.pitch, config.angle)) *
        kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.leftK)) {
    deltas.translation +=
        -glm::normalize(getRight(config.angle) * WorldHorizontal) *
        kb.trnslSpdK;
  }
  if (config.flags.isKeyboardEnabled() && input_->isKeyDown(kb.rightK)) {
    deltas.translation +=
        glm::normalize(getRight(config.angle) * WorldHorizontal) * kb.trnslSpdK;
  }
  if (config.flags.isMouseEnabled() && input_->isMouseDown(kb.trnslM) &&
      ((input_->cursordx() != 0.0f) || (input_->cursordy() != 0.0f))) {
    deltas.translation -=
        (glm::normalize(WorldHorizontal * getRight(config.angle)) *
             input_->cursordx() -
         glm::normalize(WorldHorizontal *
                        getForward(config.pitch, config.angle)) *
             input_->cursordy()) *
        kb.trnslSpdM;
  }
  deltas.translation *= static_cast<f32>(translationDelta);

  return deltas;
}

}  // namespace uinta
