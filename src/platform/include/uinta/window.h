#ifndef SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_
#define SRC_PLATFORM_INCLUDE_UINTA_WINDOW_H_

#include <string>

#include "uinta/flags.h"
#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

class Monitor;
class Platform;
class Window;

class WindowConfig {
 public:
  WindowConfig(const std::string_view title = "Uinta Window", u32 x = 0,
               u32 y = 0, u32 width = 800, u32 height = 800) noexcept
      : width_(width), height_(height), x_(x), y_(y), title_(title) {}

  u32 width() const noexcept { return width_; }

  u32 height() const noexcept { return height_; }

  u32 x() const noexcept { return x_; }

  u32 y() const noexcept { return y_; }

  const std::string& title() const noexcept { return title_; }

  const Window* apiUserPtr() const noexcept { return apiUserPtr_; }

  void apiUserPtr(const Window* apiUserPtr) noexcept {
    apiUserPtr_ = apiUserPtr;
  }

  const Monitor* monitor() const noexcept { return monitor_; }

  bool isFullscreen() const noexcept { return flags_.isFullscreen(); }

  void isFullscreen(bool v, const Monitor* monitor = nullptr) noexcept;

 private:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(FullscreenMask);

    bool isFullscreen() const noexcept { return flags_ & FullscreenMask; }
    void isFullscreen(bool v) noexcept {
      flags_ &= ~FullscreenMask;
      if (v) flags_ |= FullscreenMask;
    }

   private:
    static constexpr value_type FullscreenMask = 1 << 0;

    value_type flags_;
  } flags_;

  u32 width_;
  u32 height_;
  u32 x_;
  u32 y_;
  std::string title_;
  const Window* apiUserPtr_ = nullptr;
  const Monitor* monitor_ = nullptr;
};

class Window {
 public:
  explicit Window(const WindowConfig&, const Platform*) noexcept;

  virtual ~Window() noexcept {}

  f32 aspect() const noexcept {
    return static_cast<f32>(width_) / static_cast<f32>(height_);
  }

  u32 height() const noexcept { return height_; }

  const Monitor* monitor() const noexcept { return monitor_; }

  std::string name() const noexcept { return name_; }

  void size(u32 width, u32 height) noexcept {
    width_ = width;
    height_ = height;
  }

  void* userData() const noexcept { return userData_; }

  u32 width() const noexcept { return width_; }

  const Status& status() const noexcept { return status_; }

 protected:
  Status status_;

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
