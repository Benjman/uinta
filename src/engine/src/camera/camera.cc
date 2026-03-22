#include "uinta/camera/camera.h"

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "uinta/math/direction.h"

namespace uinta {

void Camera::update(time_t delta, const CameraInputDeltas& deltas) noexcept {
  if (!config_.flags.isAngleLock()) {
    config_.angle += deltas.angle;
    config_.angle.update(delta);
  }

  if (!config_.flags.isPitchLock()) {
    config_.pitch += deltas.pitch;
    if (config_.flags.isPitchLimit()) {
      config_.pitch = std::clamp(config_.pitch.target(), config_.pitchMin,
                                 config_.pitchMax);
    }
    config_.pitch.update(delta);
  }

  if (!config_.flags.isDistLock()) {
    config_.dist += deltas.dist;
    if (config_.flags.isDistLimit()) {
      config_.dist =
          std::clamp(config_.dist.target(), config_.dstMin, config_.dstMax);
    }
    config_.dist.update(deltas.translationDelta);
  }

  config_.target += deltas.translation;
  config_.target.update(deltas.translationDelta);

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
  result = static_cast<f32>(1.3 * std::sqrt(result));
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

}  // namespace uinta
