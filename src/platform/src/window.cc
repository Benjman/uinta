#include "uinta/window.h"

#include <algorithm>

#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

Window::Window(const WindowConfig& cfg, const Platform* platform) noexcept
    : name_(cfg.title()), width_(cfg.width()), height_(cfg.height()) {
  if (cfg.isFullscreen()) {
    monitor_ = cfg.monitor();
    if (!monitor_) {
      if (auto itr = std::find_if(
              platform->monitors().begin(), platform->monitors().end(),
              [](const auto& monitor) { return monitor.flags().isPrimary(); });
          itr != platform->monitors().end()) {
        monitor_ = &*itr;
      }
      assert(monitor_ && "Primary monitor is required.");
    }
    width_ = monitor_->width();
    height_ = monitor_->height();
  }
}

void WindowConfig::isFullscreen(bool isFullscreen,
                                const Monitor* monitor) noexcept {
  flags_.isFullscreen(isFullscreen);
  if (isFullscreen) {
    monitor_ = monitor;
    if (monitor_) {
      width_ = monitor_->width();
      height_ = monitor_->height();
    }
  }
}

}  // namespace uinta
