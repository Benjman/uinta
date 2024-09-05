#ifndef SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_

#include "uinta/desktop_platform_api.h"
#include "uinta/glfw_platform_api.h"
#include "uinta/platform.h"
#include "uinta/status.h"

namespace uinta {

class DesktopPlatform : public Platform {
 public:
  explicit DesktopPlatform(
      DesktopPlatformApi* = GlfwPlatformApi::Instance()) noexcept;

  ~DesktopPlatform() noexcept { auto _ = destroy(window_.get()); }

  DesktopPlatform(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform& operator=(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform(DesktopPlatform&&) noexcept = delete;
  DesktopPlatform& operator=(DesktopPlatform&&) noexcept = delete;

  void getAndUpdateWindowSize(i32* w, i32* h) const noexcept override {
    api_->getWindowSize(window_.get(), w, h);
    window_->size(*w, *h);
  }

  Status pollEvents() const noexcept override { return api_->pollEvents(); }

  StatusOr<time_t> runtime() const noexcept override { return api_->runtime(); }

  Status swapBuffers() const noexcept override {
    return api_->swapBuffers(window_.get());
  }

  StatusOr<void*> createWindow(Window* window) noexcept {
    return api_->createWindow(window);
  }

  Status destroy(const Window* window) const noexcept {
    return api_->destroy(window);
  }

  Status setWindowPosition(void* window, f32 x, f32 y) const noexcept {
    return api_->setWindowPosition(window, x, y);
  }

  void makeFullscreen(bool makeFullscreen, Window*,
                      const Monitor* = nullptr) noexcept override;

 private:
  DesktopPlatformApi* api_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
