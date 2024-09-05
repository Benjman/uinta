#ifndef SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_
#define SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_

#include <string>

#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class AppConfig;
class Monitor;
class Platform;
class Window;

class Window {
 public:
  explicit Window(const Platform*) noexcept;

  virtual ~Window() noexcept {}

  f32 aspect() const noexcept {
    return static_cast<f32>(width_) / static_cast<f32>(height_);
  }

  u32 height() const noexcept { return height_; }

  void monitor(Monitor* monitor) noexcept { monitor_ = monitor; }

  const Monitor* monitor() const noexcept { return monitor_; }

  std::string name() const noexcept { return name_; }

  void size(u32 width, u32 height) noexcept {
    width_ = width;
    height_ = height;
  }

  void* userData() const noexcept { return userData_; }

  u32 width() const noexcept { return width_; }

  const Status& status() const noexcept { return status_; }

  void isFullscreen(bool isFullscreen) noexcept {
    isFullscreen_ = isFullscreen;
  }

  bool isFullscreen() const noexcept { return isFullscreen_; }

  void pos(u32 x, u32 y) noexcept {
    x_ = x;
    y_ = y;
  }

  u32 x() const noexcept { return x_; }

  u32 y() const noexcept { return y_; }

 protected:
  Status status_;

  void userData(void* ptr) noexcept { userData_ = ptr; }

 private:
  std::string name_ = "Uinta Engine";
  bool isFullscreen_ = false;
  u32 height_ = 0;
  u32 width_ = 0;
  u32 x_ = 0;
  u32 y_ = 0;
  const Monitor* monitor_ = nullptr;
  void* userData_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_
