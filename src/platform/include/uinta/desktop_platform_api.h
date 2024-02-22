#ifndef SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_API_H_
#define SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_API_H_

#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/platform_signal.h"
#include "uinta/types.h"

namespace uinta {

class DesktopPlatform;
class DesktopWindow;
class GlfwPlatformApi;
class Input;
class Monitor;
class Window;

class DesktopPlatformApi {
 public:
  virtual Status clearFrontBuffer(f32, f32, f32, f32,
                                  bitflag) const noexcept = 0;

  virtual StatusOr<void*> createWindow(const Window*) noexcept = 0;

  virtual Status destroy(const Window*) const noexcept = 0;

  virtual StatusOr<std::vector<Monitor>> findMonitors() const noexcept = 0;

  virtual void getWindowSize(const Window*, i32* w, i32* h) const noexcept = 0;

  virtual Status initOpenGL() const noexcept = 0;

  virtual void onCloseRequest(const CloseRequestFunc&) noexcept = 0;

  virtual void onDebugMessage(const DebugMessageFunc&) noexcept = 0;

  virtual void onError(const ErrorFunc&) noexcept = 0;

  virtual void onFramebufferSize(const FramebufferSizeFunc&) noexcept = 0;

  virtual Status pollEvents() const noexcept = 0;

  virtual StatusOr<f64> runtime() const noexcept = 0;

  virtual Status setWindowPosition(void*, f32, f32) const noexcept = 0;

  virtual Status swapBuffers(const Window*) const noexcept = 0;

  virtual Status registerInputHandlers(const Window*, Input*) noexcept = 0;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_DESKTOP_PLATFORM_API_H_
