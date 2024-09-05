#include "uinta/window.h"

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

Window::Window(const Platform* platform) noexcept
    : monitor_(platform->primaryMonitor().value_or(nullptr)) {
  if (!monitor_) {
    status_ = NotFoundError("Primary monitor not found.");
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

}  // namespace uinta
