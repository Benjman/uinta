#include "uinta/camera/camera.h"

#include <algorithm>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "uinta/engine/engine_state.h"
#include "uinta/input.h"
#include "uinta/math/direction.h"
#include "uinta/math/spatial.h"

namespace uinta {

inline f32 angleDelta(const CameraConfig* config, time_t delta,
                      const Input* input) noexcept {
  time_t d = 0;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->anglNegK))
    d += config->anglSpdK;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->anglPosK))
    d -= config->anglSpdK;
  if (config->flags.isMouseEnabled() && input->isMouseDown(config->angleM) &&
      input->cursordx())
    d += input->cursordx() * config->anglSpdM;
  return d * delta;
}

inline f32 distDelta(const CameraConfig* config, time_t delta,
                     const Input* input) noexcept {
  time_t d = 0;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->dstIncK))
    d -= config->dstSpdK;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->dstDecK))
    d += config->dstSpdK;
  if (config->flags.isMouseEnabled() && input->mouseScrolled())
    d += input->scrolldy() * -config->dstSpdM;
  return d * delta;
}

f32 pitchDelta(const CameraConfig* config, time_t delta,
               const Input* input) noexcept {
  time_t d = 0;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->pitchPosK))
    d += config->pitchSpdK;
  if (config->flags.isKeyboardEnabled() && input->isKeyDown(config->pitchNegK))
    d -= config->pitchSpdK;
  if (config->flags.isMouseEnabled() && input->isMouseDown(config->pitchM) &&
      input->cursordy())
    d += input->cursordy() * config->pitchSpdM;
  return d * delta;
}

inline glm::vec3 translationDelta(const CameraConfig* config_, time_t delta,
                                  const Input* input) noexcept {
  glm::vec3 d(0);
  if (config_->flags.isKeyboardEnabled() && input->isKeyDown(config_->forwardK))
    d += glm::normalize(WorldHorizontal *
                        getForward(config_->pitch, config_->angle)) *
         config_->trnslSpdK;
  if (config_->flags.isKeyboardEnabled() &&
      input->isKeyDown(config_->backwardK))
    d -= glm::normalize(WorldHorizontal *
                        getForward(config_->pitch, config_->angle)) *
         config_->trnslSpdK;
  if (config_->flags.isKeyboardEnabled() && input->isKeyDown(config_->leftK))
    d += -glm::normalize(getRight(config_->angle) * WorldHorizontal) *
         config_->trnslSpdK;
  if (config_->flags.isKeyboardEnabled() && input->isKeyDown(config_->rightK))
    d += glm::normalize(getRight(config_->angle) * WorldHorizontal) *
         config_->trnslSpdK;
  if (config_->flags.isMouseEnabled() && input->isMouseDown(config_->trnslM) &&
      (input->cursordx() || input->cursordy()))
    d -= (glm::normalize(WorldHorizontal * getRight(config_->angle)) *
              input->cursordx() -
          glm::normalize(WorldHorizontal *
                         getForward(config_->pitch, config_->angle)) *
              input->cursordy()) *
         config_->trnslSpdM;
  return d * static_cast<f32>(delta);
}

void Camera::update(time_t delta, const Input* input) noexcept {
  constexpr auto FastMultiplier = 5.0;
  constexpr auto SlowMultiplier = 0.1;

  if (input->isKeyDown(KEY_LEFT_CONTROL)) delta *= FastMultiplier;
  if (input->isKeyDown(KEY_LEFT_ALT)) delta *= SlowMultiplier;

  if (!config_.flags.isAngleLock()) {
    config_.angle += angleDelta(&config_, delta, input);
    config_.angle.update(delta);
  }

  if (!config_.flags.isPitchLock()) {
    config_.pitch += pitchDelta(&config_, delta, input);
    if (config_.flags.isPitchLimit())
      config_.pitch = std::clamp(config_.pitch.target(), config_.pitchMin,
                                 config_.pitchMax);
    config_.pitch.update(delta);
  }

  delta *= translationFactor();

  if (!config_.flags.isDistLock()) {
    config_.dist += distDelta(&config_, delta, input);
    if (config_.flags.isDistLimit())
      config_.dist =
          std::clamp(config_.dist.target(), config_.dstMin, config_.dstMax);
    config_.dist.update(delta);
  }

  config_.target += translationDelta(&config_, delta, input);
  config_.target.update(delta);

  auto ar = glm::radians<f64>(config_.angle.current());
  auto pr = glm::radians<f64>(config_.pitch.current());
  auto ca = cos(ar);
  auto cp = cos(pr);
  auto sa = sin(ar);
  auto sp = sin(pr);
  position_ = {
      -sa * cp,
      sp,
      ca * cp,
  };
  position_ *= config_.dist;
  position_ +=
      static_cast<glm::vec3>(config_.target) + config_.vertOff * WorldUp;
}

f32 Camera::translationFactor() const noexcept {
  auto result = config_.dist.current();
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= config_.trnslScaleFct;
  result = std::max(config_.spdFactorMin, result);
  return result;
}

glm::mat4 Camera::viewMatrix() const noexcept {
  auto result = glm::rotate(glm::mat4(1), glm::radians(config_.pitch.current()),
                            WorldRight);
  result = glm::rotate(result, glm::radians(config_.angle.current()), WorldUp);
  result = glm::translate(result, -position_);
  return result;
}

glm::mat4 Camera::perspectiveMatrix(f32 aspectRatio) const noexcept {
  return glm::perspective(config_.fov, aspectRatio, config_.near, config_.far);
}

glm::mat4 Camera::orthographicMatrix(f32 aspectRatio) const noexcept {
  f32 halfWidth = config_.orthoSize * aspectRatio;
  f32 halfHeight = config_.orthoSize;
  return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                    config_.near, config_.far);
}

}  // namespace uinta
