#ifndef SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_WINDOW_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_WINDOW_H_

#include "uinta/desktop_platform.h"
#include "uinta/window.h"

namespace uinta {

class DesktopWindow : public Window {
 public:
  DesktopWindow(DesktopPlatform*, WindowConfig&) noexcept;

  ~DesktopWindow() noexcept;

  DesktopWindow(const DesktopWindow&) noexcept = delete;
  DesktopWindow& operator=(const DesktopWindow&) noexcept = delete;
  DesktopWindow(DesktopWindow&&) noexcept = delete;
  DesktopWindow& operator=(DesktopWindow&&) noexcept = delete;

 private:
  DesktopPlatform* platform_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_WINDOW_H_
