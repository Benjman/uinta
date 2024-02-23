#include "uinta/camera/camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "uinta/engine_state.h"
#include "uinta/math/direction.h"
#include "uinta/math/spatial.h"

namespace uinta {

void Camera::update(const EngineState& state) noexcept {
  processInput(state);
  updatePosition(state);
}

f32 Camera::translationFactor() const noexcept {
  auto result = dist_.current();
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= config_.spdFactor;
  result = std::max(config_.spdFactorMin, result);
  return result;
}

void Camera::processInput(const EngineState& state) noexcept {
  static constexpr auto FAST_MULTIPLIER = 5.0;
  static constexpr auto SLOW_MULTIPLIER = 0.1;

  auto delta = state.frameDelta();

  if (state.isKeyDown(KEY_LEFT_CONTROL)) delta *= FAST_MULTIPLIER;
  if (state.isKeyDown(KEY_LEFT_ALT)) delta *= SLOW_MULTIPLIER;

  updateAngle(delta, state.input());
  angle_.update(delta);

  updatePitch(delta, state.input());
  pitch_.update(delta);

  delta *= translationFactor();

  updateDist(delta, state.input());
  dist_.update(delta);

  updateTranslation(delta, state.input());
  target_.update(delta);
}

void Camera::updateAngle(f32 delta, const Input& input) noexcept {
  f32 d = 0;
  if (input.isKeyDown(config_.anglNegK)) d += config_.anglSpdK;
  if (input.isKeyDown(config_.anglPosK)) d -= config_.anglSpdK;
  if (input.isMouseDown(config_.angleM) && input.cursordx())
    d += input.cursordx() * config_.anglSpdM;
  angle_ += d * delta;
}

void Camera::updateDist(f32 delta, const Input& input) noexcept {
  f32 d = 0;
  if (input.isKeyDown(config_.dstIncK)) d -= config_.dstSpdK;
  if (input.isKeyDown(config_.dstDecK)) d += config_.dstSpdK;
  if (input.mouseScrolled()) d += input.scrolldy() * -config_.dstSpdM;
  dist_ += d * delta;
  if (flags_ & Masks::DistLimit)
    dist_ = std::clamp(dist_.target(), config_.dstMin, config_.dstMax);
}

void Camera::updatePitch(f32 delta, const Input& input) noexcept {
  f32 d = 0;
  if (input.isKeyDown(config_.pitchPosK)) d += config_.pitchSpdK;
  if (input.isKeyDown(config_.pitchNegK)) d -= config_.pitchSpdK;
  if (input.isMouseDown(config_.pitchM) && input.cursordy())
    d += input.cursordy() * config_.pitchSpdM;
  pitch_ += d * delta;
  if (flags_ & Masks::PitchLimit)
    pitch_ = std::clamp(pitch_.target(), config_.pitchMin, config_.pitchMax);
}

void Camera::updateTranslation(f32 delta, const Input& input) noexcept {
  glm::vec3 d(0);
  if (input.isKeyDown(config_.forwardK))
    d += glm::normalize(WorldHorizontal * getForward(pitch_, angle_)) *
         config_.trnslSpdK;
  if (input.isKeyDown(config_.backwardK))
    d -= glm::normalize(WorldHorizontal * getForward(pitch_, angle_)) *
         config_.trnslSpdK;
  if (input.isKeyDown(config_.leftK))
    d +=
        -glm::normalize(getRight(angle_) * WorldHorizontal) * config_.trnslSpdK;
  if (input.isKeyDown(config_.rightK))
    d += glm::normalize(getRight(angle_) * WorldHorizontal) * config_.trnslSpdK;
  if (input.isMouseDown(config_.trnslM) &&
      (input.cursordx() || input.cursordy()))
    d -=
        (glm::normalize(WorldHorizontal * getRight(angle_)) * input.cursordx() -
         glm::normalize(WorldHorizontal * getForward(pitch_, angle_)) *
             input.cursordy()) *
        config_.trnslSpdM;
  target_ += d * delta;
}

void Camera::updatePosition(const EngineState&) noexcept {
  auto ar = glm::radians<f64>(angle_.current());
  auto pr = glm::radians<f64>(pitch_.current());
  auto ca = cos(ar);
  auto cp = cos(pr);
  auto sa = sin(ar);
  auto sp = sin(pr);
  position_ = {
      -sa * cp,
      sp,
      ca * cp,
  };
  position_ *= dist_;
  position_ += glm::vec3(target_) + config_.vertOff * WorldUp;
}

glm::mat4 Camera::viewMatrix() const noexcept {
  auto result =
      glm::rotate(glm::mat4(1), glm::radians(pitch_.current()), WorldRight);
  result = glm::rotate(result, glm::radians(angle_.current()), WorldUp);
  result = glm::translate(result, -position_);
  return result;
}

glm::mat4 Camera::perspectiveMatrix(f32 aspectRatio) const noexcept {
  return glm::perspective(config_.fov, aspectRatio, config_.near, config_.far);
}

glm::mat4 Camera::orthographicMatrix() const noexcept {
  return glm::ortho(-config_.orthoSize, config_.orthoSize, -config_.orthoSize,
                    config_.orthoSize, config_.near, config_.far);
}

}  // namespace uinta
