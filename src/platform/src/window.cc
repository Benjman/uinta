#include "uinta/window.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include "uinta/app_config.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

struct {
  bool isFullscreen = false;
  std::string name = "Uinta Engine";
  std::array<i32, 2> position = {0, 0};
  std::array<i32, 2> size = {1920, 1080};
} constexpr defaults;

Window::Window(const Platform* platform, AppConfig* appConfig) noexcept
    : monitor_(platform->primaryMonitor().value_or(nullptr)),
      appConfig_(appConfig) {
  if (monitor_ == nullptr) {
    status_ = NotFoundError("Primary monitor not found.");
    return;
  }

  auto fullscreen = appConfig->getBoolean("platform.window.fullscreen");
  isFullscreen_ = fullscreen.value_or(defaults.isFullscreen);

  auto name = appConfig->getString("platform.window.name");
  name_ = name.value_or(defaults.name);

  std::array<i32, 2> buf;

  if (auto position =
          appConfig->getInt2("platform.window.position", buf.data());
      position.has_value()) {
    x_ = position.value()[0];
    y_ = position.value()[1];
  } else {
    x_ = defaults.position[0];
    y_ = defaults.position[1];
  }

  if (auto size = appConfig->getInt2("platform.window.size", buf.data());
      size.has_value()) {
    width_ = size.value()[0];
    height_ = size.value()[1];
  } else {
    width_ = defaults.size[0];
    height_ = defaults.size[1];
  }

  if (isFullscreen_) {
    width_ = monitor_->width();
    height_ = monitor_->height();
  } else {
    if (width_ == 0u) {
      width_ = 800;
    }
    if (height_ == 0u) {
      height_ = 600;
    }
  }

  LOG(INFO) << absl::StrFormat(
      "Window: name='%s', size=%dx%d, position=(%d,%d), fullscreen=%s", name_,
      width_, height_, x_, y_, isFullscreen_ ? "yes" : "no");
}

Window::~Window() noexcept {
  if (appConfig_ == nullptr) {
    return;
  }

  std::array<i32, 2> buf;

  buf[0] = static_cast<i32>(x_);
  buf[1] = static_cast<i32>(y_);
  if (auto status = appConfig_->setInt2("platform.window.position", buf.data());
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window position: " << status.message();
  }

  buf[0] = static_cast<i32>(width_);
  buf[1] = static_cast<i32>(height_);
  if (auto status = appConfig_->setInt2("platform.window.size", buf.data());
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window size: " << status.message();
  }

  if (auto status =
          appConfig_->setBoolean("platform.window.fullscreen", isFullscreen_);
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window fullscreen state: "
                 << status.message();
  }

  if (auto status = appConfig_->setString("platform.window.name", name_);
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window name: " << status.message();
  }
}

}  // namespace uinta
