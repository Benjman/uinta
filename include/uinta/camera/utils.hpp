#ifndef UINTA_CAMERA_UTILS_HPP
#define UINTA_CAMERA_UTILS_HPP

#include <cmath>
#include <uinta/camera/target_camera.hpp>

namespace uinta {

inline float calculateTranslationFactor(const TargetCamera& cam) {
  auto result = cam.dist.current;
  result = std::abs(result);
  result = 1.3 * std::sqrt(result);
  result *= cam.config.translationSpeedDistFactor;
  result = std::max(cam.config.translationSpeedDistFactorMin, result);
  return result;
}

}  // namespace uinta

#endif  // UINTA_CAMERA_UTILS_HPP
