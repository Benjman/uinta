#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_

#include <string>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/app_config.h"
#include "uinta/camera/camera_config.h"
#include "uinta/input/input.h"
#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"

namespace uinta {

inline CameraConfig deserialize(const AppConfig* appConfig) noexcept {
  auto getFloat = [&](const std::string& key) -> f32 {
    if (auto opt = appConfig->getFloat(key); opt.has_value()) {
      return opt.value();
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return 0.0f;
    }
  };

  auto getInt = [&](const std::string& key) -> i32 {
    if (auto opt = appConfig->getInt(key); opt.has_value()) {
      return opt.value();
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return 0;
    }
  };

  auto getSmoothFloat = [&](const std::string& key) -> SmoothFloat {
    f32 value = getFloat(key + ".value");
    f32 agility = getFloat(key + ".agility");
    return SmoothFloat(agility, value);
  };

  auto getSmoothVec3 = [&](const std::string& key) -> SmoothVec3 {
    f32 x = getFloat(key + ".x");
    f32 y = getFloat(key + ".y");
    f32 z = getFloat(key + ".z");
    f32 agility = getFloat(key + ".agility");
    return SmoothVec3(agility, x, y, z);
  };

  auto getInputKey = [&](const std::string& key) -> InputKey {
    if (auto opt = appConfig->getString(key); opt.has_value()) {
      if (auto keyOpt = Input::GetKeyFromStr(opt.value()); keyOpt.has_value()) {
        return keyOpt.value();
      } else {
        LOG(WARNING) << absl::StrFormat("Could not find InputKey from %s.",
                                        opt.value());
        return _::InputUnknown;
      }
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return _::InputUnknown;
    }
  };

  auto getMouseButton = [&](const std::string& key) -> MouseButton {
    if (auto opt = appConfig->getString(key); opt.has_value()) {
      if (auto keyOpt = Input::GetMouseButtonFromStr(opt.value());
          keyOpt.has_value()) {
        return keyOpt.value();
      } else {
        LOG(WARNING) << absl::StrFormat("Could not find MouseButton from %s.",
                                        opt.value());
        return _::InputUnknown;
      }
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return _::InputUnknown;
    }
  };

  CameraConfig config;

  if (auto opt = appConfig->getString("camera.projection"); opt.has_value()) {
    config.projection = toProjection(opt.value());
  } else {
    LOG(WARNING) << "Could not find `AppConfig(camera.projection)`.";
  }

  config.anglNegK = getInputKey("camera.mappings.keyboard.angleNegative");
  config.anglPosK = getInputKey("camera.mappings.keyboard.anglePositive");
  config.anglSpdK = getFloat("camera.angleSpeedKeyboard");
  config.anglSpdM = getFloat("camera.angleSpeedMouse");
  config.angleM = getMouseButton("camera.mappings.mouse.angle");
  config.backwardK = getInputKey("camera.mappings.keyboard.backward");
  config.dstDecK = getInputKey("camera.mappings.keyboard.distanceNegative");
  config.dstIncK = getInputKey("camera.mappings.keyboard.distancePositive");
  config.dstMax = getFloat("camera.distanceMax");
  config.dstMin = getFloat("camera.distanceMin");
  config.dstSpdK = getFloat("camera.distanceSpeedKeyboard");
  config.dstSpdM = getFloat("camera.distanceSpeedMouse");
  config.far = getFloat("camera.farPlane");
  config.forwardK = getInputKey("camera.mappings.keyboard.forward");
  config.fov = getFloat("camera.fov");
  config.angle = getSmoothFloat("camera.angle");
  config.dist = getSmoothFloat("camera.distance");
  config.flags = getInt("camera.flags");
  config.pitch = getSmoothFloat("camera.pitch");
  config.target = getSmoothVec3("camera.target");
  config.leftK = getInputKey("camera.mappings.keyboard.left");
  config.near = getFloat("camera.nearPlane");
  config.orthoSize = getFloat("camera.orthoSize");
  config.pitchM = getMouseButton("camera.mappings.mouse.pitch");
  config.pitchMax = getFloat("camera.pitchMax");
  config.pitchMin = getFloat("camera.pitchMin");
  config.pitchNegK = getInputKey("camera.mappings.keyboard.pitchNegative");
  config.pitchPosK = getInputKey("camera.mappings.keyboard.pitchPositive");
  config.pitchSpdK = getFloat("camera.pitchSpeedKeyboard");
  config.pitchSpdM = getFloat("camera.pitchSpeedMouse");
  config.rightK = getInputKey("camera.mappings.keyboard.right");
  config.spdFactorMin = getFloat("camera.speedFactorMin");
  config.trnslM = getMouseButton("camera.mappings.mouse.translation");
  config.trnslScaleFct = getFloat("camera.translationScaleFactor");
  config.trnslSpdK = getFloat("camera.translationSpeedKeyboard");
  config.trnslSpdM = getFloat("camera.translationSpeedMouse");
  config.vertOff = getFloat("camera.verticalOffset");

  return config;
}

inline void serialize(AppConfig* appConfig, CameraConfig config) noexcept {
  auto setInputKey = [&](InputKey inputKey,
                         const std::string& appConfigKey) -> void {
    if (auto opt = Input::GetKeyStr(inputKey); opt.has_value()) {
      if (auto status = appConfig->setString(appConfigKey, opt.value());
          !status.ok()) {
        LOG(WARNING) << status.message();
      }
    }
  };

  auto setMouseButton = [&](InputKey inputKey,
                            const std::string& appConfigKey) -> void {
    if (auto opt = Input::GetKeyStr(inputKey); opt.has_value()) {
      if (auto status = appConfig->setString(appConfigKey, opt.value());
          !status.ok()) {
        LOG(WARNING) << status.message();
      }
    }
  };

  auto setFloat = [&](float value, const std::string& appConfigKey) -> void {
    if (auto status = appConfig->setFloat(appConfigKey, value); !status.ok()) {
      LOG(WARNING) << status.message();
    }
  };

  auto setInt = [&](i32 value, const std::string& appConfigKey) -> void {
    if (auto status = appConfig->setInt(appConfigKey, value); !status.ok()) {
      LOG(WARNING) << status.message();
    }
  };

  auto setSmoothFloat = [&](SmoothFloat* value,
                            const std::string& appConfigKey) -> void {
    setFloat(value->target(), appConfigKey + ".value");
    setFloat(value->agility(), appConfigKey + ".agility");
  };

  auto setSmoothVec3 = [&](SmoothVec3* value,
                           const std::string& appConfigKey) -> void {
    setFloat(value->x().target(), appConfigKey + ".x");
    setFloat(value->y().target(), appConfigKey + ".y");
    setFloat(value->z().target(), appConfigKey + ".z");
    setFloat(value->x().agility(), appConfigKey + ".agility");
  };

  if (auto status = appConfig->setString("camera.projection",
                                         toString(config.projection));
      !status.ok()) {
    LOG(WARNING) << status.message();
  }

  // Order matches deserialize() for easier comparison
  setInputKey(config.anglNegK, "camera.mappings.keyboard.angleNegative");
  setInputKey(config.anglPosK, "camera.mappings.keyboard.anglePositive");
  setFloat(config.anglSpdK, "camera.angleSpeedKeyboard");
  setFloat(config.anglSpdM, "camera.angleSpeedMouse");
  setMouseButton(config.angleM, "camera.mappings.mouse.angle");
  setInputKey(config.backwardK, "camera.mappings.keyboard.backward");
  setInputKey(config.dstDecK, "camera.mappings.keyboard.distanceNegative");
  setInputKey(config.dstIncK, "camera.mappings.keyboard.distancePositive");
  setFloat(config.dstMax, "camera.distanceMax");
  setFloat(config.dstMin, "camera.distanceMin");
  setFloat(config.dstSpdK, "camera.distanceSpeedKeyboard");
  setFloat(config.dstSpdM, "camera.distanceSpeedMouse");
  setFloat(config.far, "camera.farPlane");
  setInputKey(config.forwardK, "camera.mappings.keyboard.forward");
  setFloat(config.fov, "camera.fov");
  setSmoothFloat(&config.angle, "camera.angle");
  setSmoothFloat(&config.dist, "camera.distance");
  setInt(config.flags, "camera.flags");
  setSmoothFloat(&config.pitch, "camera.pitch");
  setSmoothVec3(&config.target, "camera.target");
  setInputKey(config.leftK, "camera.mappings.keyboard.left");
  setFloat(config.near, "camera.nearPlane");
  setFloat(config.orthoSize, "camera.orthoSize");
  setMouseButton(config.pitchM, "camera.mappings.mouse.pitch");
  setFloat(config.pitchMax, "camera.pitchMax");
  setFloat(config.pitchMin, "camera.pitchMin");
  setInputKey(config.pitchNegK, "camera.mappings.keyboard.pitchNegative");
  setInputKey(config.pitchPosK, "camera.mappings.keyboard.pitchPositive");
  setFloat(config.pitchSpdK, "camera.pitchSpeedKeyboard");
  setFloat(config.pitchSpdM, "camera.pitchSpeedMouse");
  setInputKey(config.rightK, "camera.mappings.keyboard.right");
  setFloat(config.spdFactorMin, "camera.speedFactorMin");
  setMouseButton(config.trnslM, "camera.mappings.mouse.translation");
  setFloat(config.trnslScaleFct, "camera.translationScaleFactor");
  setFloat(config.trnslSpdK, "camera.translationSpeedKeyboard");
  setFloat(config.trnslSpdM, "camera.translationSpeedMouse");
  setFloat(config.vertOff, "camera.verticalOffset");

  LOG(INFO) << absl::StrFormat(
      "Camera properties serialized: projection=%s, target=(%.2f,%.2f,%.2f), "
      "distance=%.2f, pitch=%.2f, angle=%.2f, fov=%.2f",
      toString(config.projection), config.target.x().target(),
      config.target.y().target(), config.target.z().target(),
      config.dist.target(), config.pitch.target(), config.angle.target(),
      config.fov);
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_
