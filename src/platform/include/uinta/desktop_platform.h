#ifndef SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_

#include "uinta/desktop_platform_api.h"
#include "uinta/glfw_platform_api.h"
#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/platform_signal.h"

namespace uinta {

class DesktopPlatform : public Platform {
 public:
  explicit DesktopPlatform(
      WindowConfig = {},
      DesktopPlatformApi* = GlfwPlatformApi::Instance()) noexcept;

  ~DesktopPlatform() noexcept override;

  DesktopPlatform(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform& operator=(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform(DesktopPlatform&&) noexcept = delete;
  DesktopPlatform& operator=(DesktopPlatform&&) noexcept = delete;

  Status clearFrontBuffer(f32 r, f32 g, f32 b, f32 a,
                          u32 clearMask) const noexcept override {
    return api_->clearFrontBuffer(r, g, b, a, clearMask);
  }

  void getAndUpdateWindowSize(i32* w, i32* h) const noexcept override {
    api_->getWindowSize(window_.get(), w, h);
    window_->size(*w, *h);
  }

  void onCloseRequest(const CloseRequestFunc& func) noexcept override {
    api_->onCloseRequest(func);
  }

  void onDebugMessage(const DebugMessageFunc& func) noexcept override {
    api_->onDebugMessage(func);
  }

  void onError(const ErrorFunc& func) noexcept override { api_->onError(func); }

  void onFramebufferSize(const FramebufferSizeFunc& func) noexcept override {
    api_->onFramebufferSize(func);
  }

  Status pollEvents() const noexcept override { return api_->pollEvents(); }

  StatusOr<f64> runtime() const noexcept override { return api_->runtime(); }

  Status swapBuffers() const noexcept override {
    return api_->swapBuffers(window_.get());
  }

  StatusOr<void*> createWindow(const Window* window) noexcept {
    return api_->createWindow(window);
  }

  Status destroy(const Window* window) const noexcept {
    return api_->destroy(window);
  }

  Status setWindowPosition(void* window, f32 x, f32 y) const noexcept {
    return api_->setWindowPosition(window, x, y);
  }

  Status registerInputHandlers(Input* input) const noexcept override {
    return api_->registerInputHandlers(window_.get(), input);
  }

 private:
  DesktopPlatformApi* api_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
