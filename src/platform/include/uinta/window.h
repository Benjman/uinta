#ifndef SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_
#define SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_

#include <string>

#include "uinta/types.h"

namespace uinta {

class DesktopWindow;
class Monitor;
class Platform;

class WindowConfig {
 public:
  WindowConfig(const std::string_view title = "Uinta Window", u32 x = 0,
               u32 y = 0, u32 width = 800, u32 height = 800,
               bitflag8 flags = 0) noexcept
      : width_(width),
        height_(height),
        x_(x),
        y_(y),
        flags_(flags),
        title_(title) {}

  u32 width() const noexcept { return width_; }

  u32 height() const noexcept { return height_; }

  u32 x() const noexcept { return x_; }

  u32 y() const noexcept { return y_; }

  const std::string& title() const noexcept { return title_; }

  const DesktopWindow* apiUserPtr() const noexcept { return apiUserPtr_; }

  void apiUserPtr(const DesktopWindow* apiUserPtr) noexcept {
    apiUserPtr_ = apiUserPtr;
  }

  const Monitor* monitor() const noexcept { return monitor_; }

  bool isFullscreen() const noexcept { return flags_ & Fullscreen; }

  void isFullscreen(bool v, const Monitor* monitor = nullptr) noexcept;

 private:
  bitflag8 Fullscreen = 1 << 0;

  u32 width_;
  u32 height_;
  u32 x_;
  u32 y_;
  bitflag8 flags_;
  std::string title_;
  const DesktopWindow* apiUserPtr_ = nullptr;
  const Monitor* monitor_ = nullptr;
};

class Window {
 public:
  explicit Window(const WindowConfig&, const Platform*) noexcept;

  virtual ~Window() noexcept {}

  u32 height() const noexcept { return height_; }

  f32 heightf() const noexcept { return height_; }

  void height(u32 h) noexcept { height_ = h; }

  const Monitor* monitor() const noexcept { return monitor_; }

  std::string name() const noexcept { return name_; }

  void* userData() const noexcept { return userData_; }

  u32 width() const noexcept { return width_; }

  f32 widthf() const noexcept { return width_; }

  void width(u32 w) noexcept { width_ = w; }

 protected:
  void userData(void* ptr) noexcept { userData_ = ptr; }

 private:
  std::string name_;
  u32 width_;
  u32 height_;
  const Monitor* monitor_ = nullptr;
  void* userData_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_
