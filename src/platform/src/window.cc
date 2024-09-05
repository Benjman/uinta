#include "uinta/window.h"

#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

Window::Window(const Platform* platform) noexcept {
  if (isFullscreen_) {
    if (platform->primaryMonitor().has_value()) {
      monitor_ = platform->primaryMonitor().value();
      if (auto itr = std::find_if(
              platform->monitors().begin(), platform->monitors().end(),
              [](const auto& monitor) { return monitor.flags().isPrimary(); });
          itr != platform->monitors().end()) {
        monitor_ = &*itr;
      }
      assert(monitor_ && "Primary monitor is required.");
    } else {
      status_ = NotFoundError("Primary monitor not found.");
      return;
    }
    width_ = monitor_->width();
    height_ = monitor_->height();
  }
}

}  // namespace uinta
