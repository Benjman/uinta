#include "uinta/camera.h"

#include "absl/log/log.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "uinta/engine_state.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/math.h"

namespace uinta {

Camera::Camera(Camera&& other) noexcept
    : config_(other.config_),
      position_(other.position_),
      target_(other.target_),
      angle_(other.angle_),
      dist_(other.dist_),
      pitch_(other.pitch_),
      flags_(other.flags_),
      logger_(other.logger_) {
  other.config_ = {};
  other.position_ = glm::vec3(0);
  other.target_ = glm::vec3(0);
  other.angle_ = 0;
  other.dist_ = 0;
  other.pitch_ = 0;
  other.flags_ = 0;
}

Camera& Camera::operator=(Camera&& other) noexcept {
  if (this != &other) {
    config_ = other.config_;
    position_ = other.position_;
    target_ = other.target_;
    angle_ = other.angle_;
    dist_ = other.dist_;
    pitch_ = other.pitch_;
    flags_ = other.flags_;
    logger_ = other.logger_;

    other.config_ = {};
    other.position_ = glm::vec3(0);
    other.target_ = glm::vec3(0);
    other.angle_ = 0;
    other.dist_ = 0;
    other.pitch_ = 0;
    other.flags_ = 0;
  }
  return *this;
}

void Camera::update(const EngineState& state, const Input& input) noexcept {
  if (!state.isNewFrame()) return;
  processInput(state, input);
  updatePosition(state, input);
  logger_.update(this, state, input);
}

f32 Camera::translationFactor() const noexcept {
  auto result = dist_.current();
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= config_.spdFactor;
  result = std::max(config_.spdFactorMin, result);
  return result;
}

void Camera::processInput(const EngineState& state,
                          const Input& input) noexcept {
  static constexpr auto FAST_MULTIPLIER = 5.0;
  static constexpr auto SLOW_MULTIPLIER = 0.1;

  f32 delta = state.delta();

  if (input.isKeyDown(KEY_LEFT_CONTROL)) delta *= FAST_MULTIPLIER;
  if (input.isKeyDown(KEY_LEFT_ALT)) delta *= SLOW_MULTIPLIER;

  updateAngle(delta, input);
  angle_.update(delta);

  updatePitch(delta, input);
  pitch_.update(delta);

  delta *= translationFactor();

  updateDist(delta, input);
  dist_.update(delta);

  updateTranslation(delta, input);
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
  if (flags_ & DistLimitMask)
    dist_ = std::clamp(dist_.target(), config_.dstMin, config_.dstMax);
}

void Camera::updatePitch(f32 delta, const Input& input) noexcept {
  f32 d = 0;
  if (input.isKeyDown(config_.pitchPosK)) d += config_.pitchSpdK;
  if (input.isKeyDown(config_.pitchNegK)) d -= config_.pitchSpdK;
  if (input.isMouseDown(config_.pitchM) && input.cursordy())
    d += input.cursordy() * config_.pitchSpdM;
  pitch_ += d * delta;
  if (flags_ & PitchLimitMask)
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

void Camera::updatePosition(const EngineState&, const Input&) noexcept {
  auto ar = glm::radians(angle_.current());
  auto pr = glm::radians(pitch_.current());
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
  glm::mat4 result =
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

void Camera::CameraLogger::log(const Camera* camera) const noexcept {
  LOG(INFO) << StrFormat(
      "t(%.2f %.2f %.2f)\tp(%.2f %.2f %.2f)\td%.2f\ta%.2f\tp%.2f",
      camera->target_.x(), camera->target_.y(), camera->target_.z(),
      camera->position_.x, camera->position_.y, camera->position_.z,
      camera->dist_, camera->angle_, camera->pitch_);
}

}  // namespace uinta
