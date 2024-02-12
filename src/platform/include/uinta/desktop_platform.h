#ifndef SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_

#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/types.h"
#include "uinta/window.h"

namespace uinta {

class DesktopPlatform;
class DesktopWindow;
class GlfwPlatformApi;
class Monitor;

class DesktopPlatformApi {
 public:
  virtual Status clearFrontBuffer(f32, f32, f32, f32,
                                  bitflag) const noexcept = 0;

  virtual StatusOr<void*> createWindow(const Window*) noexcept = 0;

  virtual Status destroy(const Window*) const noexcept = 0;

  virtual StatusOr<std::vector<Monitor>> findMonitors() const noexcept = 0;

  virtual Status initOpenGL() const noexcept = 0;

  virtual void onCloseRequest(const CloseRequestFunc&) noexcept = 0;

  virtual void onDebugMessage(const DebugMessageFunc&) noexcept = 0;

  virtual void onError(const ErrorFunc&) noexcept = 0;

  virtual void onFramebufferSize(const FramebufferSizeFunc&) noexcept = 0;

  virtual Status pollEvents() const noexcept = 0;

  virtual StatusOr<f64> runtime() const noexcept = 0;

  virtual Status setWindowPosition(void*, f32, f32) const noexcept = 0;

  virtual Status swapBuffers(const Window*) const noexcept = 0;
};

class DesktopPlatform : public Platform {
 public:
  explicit DesktopPlatform(WindowConfig = WindowConfig()) noexcept;

  ~DesktopPlatform() noexcept = default;

  DesktopPlatform(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform& operator=(const DesktopPlatform&) noexcept = delete;
  DesktopPlatform(DesktopPlatform&&) noexcept = delete;
  DesktopPlatform& operator=(DesktopPlatform&&) noexcept = delete;

  Status clearFrontBuffer(f32, f32, f32, f32, bitflag) const noexcept override;

  void onCloseRequest(const CloseRequestFunc&) noexcept override;

  void onDebugMessage(const DebugMessageFunc&) noexcept override;

  void onError(const ErrorFunc&) noexcept override;

  void onFramebufferSize(const FramebufferSizeFunc&) noexcept override;

  Status pollEvents() const noexcept override;

  StatusOr<f64> runtime() const noexcept override;

  Status swapBuffers() const noexcept override;

  StatusOr<void*> createWindow(const Window*) noexcept;

  Status destroy(const Window*) const noexcept;

  Status setWindowPosition(void* window, f32 x, f32 y) const noexcept;

  const Status& status() const noexcept { return status_; }

 private:
  GlfwPlatformApi* api_;
  Status status_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_H_
