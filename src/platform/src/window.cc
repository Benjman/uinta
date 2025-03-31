#include "uinta/window.h"

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/app_config.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

Window::Window(const Platform* platform, AppConfig* appConfig) noexcept
    : monitor_(platform->primaryMonitor().value_or(nullptr)),
      appConfig_(appConfig) {
  if (!monitor_) {
    status_ = NotFoundError("Primary monitor not found.");
    return;
  }

  constexpr auto FullscreenKey = "platform.window.fullscreen";
  if (auto fullscreen = appConfig->getBoolean(FullscreenKey);
      fullscreen.has_value()) {
    isFullscreen_ = fullscreen.value();
  } else {
    status_ = NotFoundError(
        absl::StrFormat("AppConfig not found for key '%s'.", FullscreenKey));
    return;
  }

  constexpr auto NameKey = "platform.window.name";
  if (auto name = appConfig->getString(NameKey); name.has_value()) {
    name_ = name.value();
  } else {
    status_ = NotFoundError(
        absl::StrFormat("AppConfig not found for key '%s'.", NameKey));
    return;
  }

  std::array<i32, 2> buf;

  constexpr auto PositionKey = "platform.window.position";
  if (auto opt = appConfig->getInt2(PositionKey, buf.data()); opt.has_value()) {
    x_ = buf[0];
    y_ = buf[1];
  } else {
    status_ = NotFoundError(
        absl::StrFormat("AppConfig not found for key '%s'.", PositionKey));
    return;
  }

  constexpr auto SizeKey = "platform.window.size";
  if (auto opt = appConfig->getInt2(SizeKey, buf.data()); opt.has_value()) {
    width_ = buf[0];
    height_ = buf[1];
  } else {
    status_ = NotFoundError(
        absl::StrFormat("AppConfig not found for key '%s'.", SizeKey));
    return;
  }

  if (isFullscreen_) {
    width_ = monitor_->width();
    height_ = monitor_->height();
  } else {
    if (!width_) {
      width_ = 800;
    }
    if (!height_) {
      height_ = 600;
    }
  }

  LOG(INFO) << absl::StrFormat(
      "Window: name='%s', size=%dx%d, position=(%d,%d), fullscreen=%s", name_,
      width_, height_, x_, y_, isFullscreen_ ? "yes" : "no");
}

Window::~Window() noexcept {
  if (!appConfig_) {
    return;
  }

  std::array<i32, 2> buf;

  buf[0] = x_;
  buf[1] = y_;
  if (auto status = appConfig_->setInt2("platform.window.position", buf.data());
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window position: " << status.message();
  }

  buf[0] = width_;
  buf[1] = height_;
  if (auto status = appConfig_->setInt2("platform.window.size", buf.data());
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window size: " << status.message();
  }

  if (auto status = appConfig_->setBoolean("platform.window.fullscreen",
                                           isFullscreen_);
      !status.ok()) {
    LOG(WARNING) << "Failed to serialize window fullscreen state: "
                 << status.message();
  }

  appConfig_->flush();

  LOG(INFO) << absl::StrFormat(
      "Window properties serialized: size=%dx%d, position=(%d,%d), "
      "fullscreen=%s",
      width_, height_, x_, y_, isFullscreen_ ? "yes" : "no");
}

}  // namespace uinta