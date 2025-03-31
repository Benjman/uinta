#include "uinta/window.h"

#include <absl/status/status.h>

#include "absl/strings/str_format.h"
#include "uinta/app_config.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

Window::Window(const Platform* platform, const AppConfig* appConfig) noexcept {
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

  if (auto opt = platform->primaryMonitor(); opt.has_value()) {
    monitor_ = opt.value();

    if (!width_) {
      width_ = monitor_->width();
    }

    if (height_ == 0) {
      height_ = monitor_->height();
    }

    if (!isFullscreen_) {
      monitor_ = nullptr;
    }
  } else {
    status_ = NotFoundError("Primary monitor nout found.");
  }
}

}  // namespace uinta
