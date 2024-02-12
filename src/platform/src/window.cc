#include "uinta/window.h"

#include <algorithm>

#include "uinta/platform.h"

namespace uinta {

Window::Window(const WindowConfig& cfg, const Platform* platform) noexcept
    : name_(cfg.title()), width_(cfg.width()), height_(cfg.height()) {
  if (cfg.isFullscreen()) {
    monitor_ = cfg.monitor();
    if (!monitor_) {
      // Find primary monitor:
      const auto isMonitorPrimary = [](const auto& monitor) {
        return monitor.isPrimary();
      };
      if (auto itr = std::find_if(platform->monitors().begin(),
                                  platform->monitors().end(), isMonitorPrimary);
          itr != platform->monitors().end()) {
        monitor_ = &*itr;
      }
    }
    width_ = monitor_->width();
    height_ = monitor_->height();
  }
}

void WindowConfig::isFullscreen(bool v, const Monitor* monitor) noexcept {
  flags_ &= ~Fullscreen;
  if (v) {
    flags_ |= Fullscreen;
    monitor_ = monitor;
    if (monitor_) {
      width_ = monitor_->width();
      height_ = monitor_->height();
    }
  }
}

}  // namespace uinta
