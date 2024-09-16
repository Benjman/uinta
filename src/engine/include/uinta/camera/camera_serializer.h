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
  auto getFloat = [&](std::string key) -> f32 {
    if (auto opt = appConfig->getFloat(key); opt.has_value()) {
      return opt.value();
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return SmoothFloat(0);
    }
  };

  auto getSmoothFloat = [&](std::string key) -> SmoothFloat {
    f32 buf[2];
    if (auto opt = appConfig->getFloat2(key, buf); opt.has_value()) {
      return SmoothFloat(buf[1], buf[0]);
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return SmoothFloat(0);
    }
  };

  auto getSmoothVec3 = [&](std::string key) -> SmoothVec3 {
    f32 buf[4];
    if (auto opt = appConfig->getFloat4(key, buf); opt.has_value()) {
      return SmoothVec3(buf[3], buf[0], buf[1], buf[2]);
    } else {
      LOG(WARNING) << absl::StrFormat("Could not find `AppConfig(%s)`.", key);
      return SmoothVec3(0);
    }
  };

  auto getInputKey = [&](std::string key) -> InputKey {
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

  auto getMouseButton = [&](std::string key) -> MouseButton {
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
  config.flags = getFloat("camera.flags");
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
  auto setInputKey = [&](InputKey inputKey, std::string appConfigKey) -> void {
    if (auto opt = Input::GetKeyStr(inputKey); opt.has_value()) {
      if (auto status = appConfig->setString(appConfigKey, opt.value());
          !status.ok()) {
        LOG(WARNING) << status.message();
      }
    }
  };

  auto setMouseButton = [&](InputKey inputKey,
                            std::string appConfigKey) -> void {
    if (auto opt = Input::GetKeyStr(inputKey); opt.has_value()) {
      if (auto status = appConfig->setString(appConfigKey, opt.value());
          !status.ok()) {
        LOG(WARNING) << status.message();
      }
    }
  };

  auto setFloat = [&](float value, std::string appConfigKey) -> void {
    if (auto status = appConfig->setFloat(appConfigKey, value); !status.ok()) {
      LOG(WARNING) << status.message();
    }
  };

  auto setSmoothFloat = [&](SmoothFloat* value,
                            std::string appConfigKey) -> void {
    f32 buf[2] = {value->target(), value->agility()};
    if (auto status = appConfig->setFloat2(appConfigKey, buf); !status.ok()) {
      LOG(WARNING) << status.message();
    }
  };

  auto setSmoothVec3 = [&](SmoothVec3* value,
                           std::string appConfigKey) -> void {
    f32 buf[4] = {value->x().target(), value->y().target(), value->z().target(),
                  value->x().agility()};
    if (auto status = appConfig->setFloat4(appConfigKey, buf); !status.ok()) {
      LOG(WARNING) << status.message();
    }
  };

  if (auto status = appConfig->setString("camera.projection",
                                         toString(config.projection));
      !status.ok()) {
    LOG(WARNING) << status.message();
  }

  setFloat(config.anglSpdK, "camera.angleSpeedKeyboard");
  setFloat(config.anglSpdM, "camera.angleSpeedMouse");
  setFloat(config.dstMax, "camera.distanceMax");
  setFloat(config.dstMin, "camera.distanceMin");
  setFloat(config.dstSpdK, "camera.distanceSpeedKeyboard");
  setFloat(config.dstSpdM, "camera.distanceSpeedMouse");
  setFloat(config.far, "camera.farPlane");
  setFloat(config.fov, "camera.fov");
  setFloat(config.flags, "camera.flags");
  setFloat(config.near, "camera.nearPlane");
  setFloat(config.orthoSize, "camera.orthoSize");
  setFloat(config.pitchMax, "camera.pitchMax");
  setFloat(config.pitchMin, "camera.pitchMin");
  setFloat(config.pitchSpdK, "camera.pitchSpeedKeyboard");
  setFloat(config.pitchSpdM, "camera.pitchSpeedMouse");
  setFloat(config.spdFactorMin, "camera.speedFactorMin");
  setFloat(config.trnslScaleFct, "camera.translationScaleFactor");
  setFloat(config.trnslSpdK, "camera.translationSpeedKeyboard");
  setFloat(config.trnslSpdM, "camera.translationSpeedMouse");
  setFloat(config.vertOff, "camera.verticalOffset");

  setInputKey(config.anglPosK, "camera.mappings.keyboard.anglePositive");
  setInputKey(config.backwardK, "camera.mappings.keyboard.backward");
  setInputKey(config.dstDecK, "camera.mappings.keyboard.distanceNegative");
  setInputKey(config.dstIncK, "camera.mappings.keyboard.distancePositive");
  setInputKey(config.forwardK, "camera.mappings.keyboard.forward");
  setInputKey(config.leftK, "camera.mappings.keyboard.left");
  setInputKey(config.pitchNegK, "camera.mappings.keyboard.pitchNegative");
  setInputKey(config.pitchPosK, "camera.mappings.keyboard.pitchPositive");
  setInputKey(config.rightK, "camera.mappings.keyboard.right");

  setMouseButton(config.angleM, "camera.mappings.mouse.angle");
  setMouseButton(config.pitchM, "camera.mappings.mouse.pitch");
  setMouseButton(config.trnslM, "camera.mappings.mouse.translation");

  setSmoothFloat(&config.angle, "camera.angle");
  setSmoothFloat(&config.dist, "camera.distance");
  setSmoothFloat(&config.pitch, "camera.pitch");
  setSmoothVec3(&config.target, "camera.target");
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_
