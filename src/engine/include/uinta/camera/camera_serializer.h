#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <string>

#include "uinta/app_config.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_keybindings.h"
#include "uinta/input.h"
#include "uinta/input/fwd.h"
#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"

namespace uinta {

inline CameraConfig deserialize(const AppConfig* appConfig) noexcept {
  auto getSmoothFloat = [&](const std::string& key, f32 fallbackValue,
                            f32 fallbackAgility) -> SmoothFloat {
    return SmoothFloat(
        appConfig->getFloat(key + ".agility").value_or(fallbackAgility),
        appConfig->getFloat(key + ".value").value_or(fallbackValue));
  };

  auto getSmoothVec3 = [&](const std::string& key, f32 fallbackX, f32 fallbackY,
                           f32 fallbackZ, f32 fallbackAgility) -> SmoothVec3 {
    f32 x = appConfig->getFloat(key + ".x").value_or(fallbackX);
    f32 y = appConfig->getFloat(key + ".y").value_or(fallbackY);
    f32 z = appConfig->getFloat(key + ".z").value_or(fallbackZ);
    f32 agility =
        appConfig->getFloat(key + ".agility").value_or(fallbackAgility);
    return SmoothVec3(agility, x, y, z);
  };

  CameraConfig config;

  constexpr auto DefaultFlags = CameraConfig::Flags::DefaultFlags;
  config.dstMax = appConfig->getFloat("camera.distanceMax").value_or(50);
  config.dstMin = appConfig->getFloat("camera.distanceMin").value_or(0.2);
  config.angle = getSmoothFloat("camera.angle", 315.0, 8.0);
  config.dist = getSmoothFloat("camera.distance", 50.0, 8.0);
  config.flags = appConfig->getInt("camera.flags").value_or(DefaultFlags);
  config.pitch = getSmoothFloat("camera.pitch", 45.0, 8.0);
  config.target = getSmoothVec3("camera.target", 0, 0, 0, 8.0);
  config.pitchMax = appConfig->getFloat("camera.pitchMax").value_or(90);
  config.pitchMin = appConfig->getFloat("camera.pitchMin").value_or(0.5);
  config.spdFactorMin =
      appConfig->getFloat("camera.speedFactorMin").value_or(0.66);
  config.trnslScaleFct =
      appConfig->getFloat("camera.translationScaleFactor").value_or(0.3);
  config.vertOff = appConfig->getFloat("camera.verticalOffset").value_or(0);

  return config;
}

inline CameraKeybindings deserializeKeybindings(
    const AppConfig* appConfig) noexcept {
  auto getInputKey = [&](const std::string& key,
                         InputKey fallback) -> InputKey {
    if (auto opt = appConfig->getString(key); opt.has_value()) {
      if (auto keyOpt = Input::GetKeyFromStr(opt.value()); keyOpt.has_value()) {
        return keyOpt.value();
      }
      LOG(WARNING) << absl::StrFormat("Could not find InputKey from %s.",
                                      opt.value());
    }
    return fallback;
  };

  auto getMouseButton = [&](const std::string& key,
                            MouseButton fallback) -> MouseButton {
    if (auto opt = appConfig->getString(key); opt.has_value()) {
      if (auto keyOpt = Input::GetMouseButtonFromStr(opt.value());
          keyOpt.has_value()) {
        return keyOpt.value();
      }
      LOG(WARNING) << absl::StrFormat("Could not find MouseButton from %s.",
                                      opt.value());
    }
    return fallback;
  };

  CameraKeybindings kb;

  // Keyboard keys
  kb.anglNegK = getInputKey("camera.mappings.keyboard.angleNegative", KEY_E);
  kb.anglPosK = getInputKey("camera.mappings.keyboard.anglePositive", KEY_Q);
  kb.dstDecK = getInputKey("camera.mappings.keyboard.distanceNegative", KEY_C);
  kb.dstIncK = getInputKey("camera.mappings.keyboard.distancePositive", KEY_V);
  kb.pitchNegK = getInputKey("camera.mappings.keyboard.pitchNegative", KEY_F);
  kb.pitchPosK = getInputKey("camera.mappings.keyboard.pitchPositive", KEY_R);
  kb.backwardK = getInputKey("camera.mappings.keyboard.backward", KEY_S);
  kb.forwardK = getInputKey("camera.mappings.keyboard.forward", KEY_W);
  kb.leftK = getInputKey("camera.mappings.keyboard.left", KEY_A);
  kb.rightK = getInputKey("camera.mappings.keyboard.right", KEY_D);

  // Mouse buttons
  kb.angleM = getMouseButton("camera.mappings.mouse.angle", MOUSE_BUTTON_RIGHT);
  kb.pitchM = getMouseButton("camera.mappings.mouse.pitch", MOUSE_BUTTON_RIGHT);
  kb.trnslM =
      getMouseButton("camera.mappings.mouse.translation", MOUSE_BUTTON_MIDDLE);

  // Speed settings
  kb.anglSpdK = appConfig->getFloat("camera.angleSpeedKeyboard").value_or(90);
  kb.anglSpdM = appConfig->getFloat("camera.angleSpeedMouse").value_or(20);
  kb.dstSpdK = appConfig->getFloat("camera.distanceSpeedKeyboard").value_or(15);
  kb.dstSpdM = appConfig->getFloat("camera.distanceSpeedMouse").value_or(30);
  kb.pitchSpdK = appConfig->getFloat("camera.pitchSpeedKeyboard").value_or(90);
  kb.pitchSpdM = appConfig->getFloat("camera.pitchSpeedMouse").value_or(20);
  kb.trnslSpdK =
      appConfig->getFloat("camera.translationSpeedKeyboard").value_or(10);
  kb.trnslSpdM =
      appConfig->getFloat("camera.translationSpeedMouse").value_or(2);

  return kb;
}

inline void serialize(AppConfig* appConfig, CameraConfig config) noexcept {
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

  setFloat(config.dstMax, "camera.distanceMax");
  setFloat(config.dstMin, "camera.distanceMin");
  setSmoothFloat(&config.angle, "camera.angle");
  setSmoothFloat(&config.dist, "camera.distance");
  setInt(config.flags, "camera.flags");
  setSmoothFloat(&config.pitch, "camera.pitch");
  setSmoothVec3(&config.target, "camera.target");
  setFloat(config.pitchMax, "camera.pitchMax");
  setFloat(config.pitchMin, "camera.pitchMin");
  setFloat(config.spdFactorMin, "camera.speedFactorMin");
  setFloat(config.trnslScaleFct, "camera.translationScaleFactor");
  setFloat(config.vertOff, "camera.verticalOffset");

  LOG(INFO) << absl::StrFormat(
      "Camera properties serialized: target=(%.2f,%.2f,%.2f), distance=%.2f, "
      "pitch=%.2f, angle=%.2f",
      config.target.x().target(), config.target.y().target(),
      config.target.z().target(), config.dist.target(), config.pitch.target(),
      config.angle.target());
}

inline void serializeKeybindings(AppConfig* appConfig,
                                 const CameraKeybindings& kb) noexcept {
  auto setInputKey = [&](InputKey inputKey,
                         const std::string& appConfigKey) -> void {
    if (auto opt = Input::GetKeyStr(inputKey); opt.has_value()) {
      if (auto status = appConfig->setString(appConfigKey, opt.value());
          !status.ok()) {
        LOG(WARNING) << status.message();
      }
    }
  };

  auto setMouseButton = [&](MouseButton button,
                            const std::string& appConfigKey) -> void {
    if (auto opt = Input::GetMouseButtonStr(button); opt.has_value()) {
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

  // Keyboard keys
  setInputKey(kb.anglNegK, "camera.mappings.keyboard.angleNegative");
  setInputKey(kb.anglPosK, "camera.mappings.keyboard.anglePositive");
  setInputKey(kb.dstDecK, "camera.mappings.keyboard.distanceNegative");
  setInputKey(kb.dstIncK, "camera.mappings.keyboard.distancePositive");
  setInputKey(kb.pitchNegK, "camera.mappings.keyboard.pitchNegative");
  setInputKey(kb.pitchPosK, "camera.mappings.keyboard.pitchPositive");
  setInputKey(kb.backwardK, "camera.mappings.keyboard.backward");
  setInputKey(kb.forwardK, "camera.mappings.keyboard.forward");
  setInputKey(kb.leftK, "camera.mappings.keyboard.left");
  setInputKey(kb.rightK, "camera.mappings.keyboard.right");

  // Mouse buttons
  setMouseButton(kb.angleM, "camera.mappings.mouse.angle");
  setMouseButton(kb.pitchM, "camera.mappings.mouse.pitch");
  setMouseButton(kb.trnslM, "camera.mappings.mouse.translation");

  // Speed settings
  setFloat(kb.anglSpdK, "camera.angleSpeedKeyboard");
  setFloat(kb.anglSpdM, "camera.angleSpeedMouse");
  setFloat(kb.dstSpdK, "camera.distanceSpeedKeyboard");
  setFloat(kb.dstSpdM, "camera.distanceSpeedMouse");
  setFloat(kb.pitchSpdK, "camera.pitchSpeedKeyboard");
  setFloat(kb.pitchSpdM, "camera.pitchSpeedMouse");
  setFloat(kb.trnslSpdK, "camera.translationSpeedKeyboard");
  setFloat(kb.trnslSpdM, "camera.translationSpeedMouse");
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_SERIALIZER_H_
