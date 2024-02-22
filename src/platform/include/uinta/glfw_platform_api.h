#ifndef SRC_PLATFORM_INCLUDE_UINTA_GLFW_PLATFORM_API_H_
#define SRC_PLATFORM_INCLUDE_UINTA_GLFW_PLATFORM_API_H_

#include <vector>

#include "uinta/desktop_platform.h"
#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

class Input;
class Monitor;

class GlfwPlatformApi final : public DesktopPlatformApi {
 public:
  static GlfwPlatformApi* GetInstance() noexcept {
    static GlfwPlatformApi instance;
    return &instance;
  }

  Status clearFrontBuffer(f32 r, f32 g, f32 b, f32 a,
                          bitflag clearMask) const noexcept override;

  StatusOr<void*> createWindow(const Window*) noexcept override;

  Status destroy(const Window*) const noexcept override;

  StatusOr<std::vector<Monitor>> findMonitors() const noexcept override;

  Status initOpenGL() const noexcept override;

  void onCloseRequest(const CloseRequestFunc&) noexcept override;

  void onDebugMessage(const DebugMessageFunc&) noexcept override;

  void onError(const ErrorFunc&) noexcept override;

  void onFramebufferSize(const FramebufferSizeFunc&) noexcept override;

  Status pollEvents() const noexcept override;

  StatusOr<f64> runtime() const noexcept override;

  Status setWindowPosition(void* winodw, f32 x, f32 y) const noexcept override;

  Status swapBuffers(const Window*) const noexcept override;

  Status registerInputHandlers(const Window*, Input*) noexcept override;

 private:
  GlfwPlatformApi() noexcept = default;
  ~GlfwPlatformApi() noexcept = default;
  GlfwPlatformApi(const GlfwPlatformApi&) noexcept = delete;
  GlfwPlatformApi& operator=(const GlfwPlatformApi&) noexcept = delete;
  GlfwPlatformApi(GlfwPlatformApi&&) noexcept = delete;
  GlfwPlatformApi& operator=(GlfwPlatformApi&&) noexcept = delete;

  struct WindowData final {
    CloseRequestFunc onCloseRequest;
    DebugMessageFunc onDebugMessage;
    ErrorFunc onError;
    FramebufferSizeFunc onFramebufferSize;
    Input* input;
  } windowData_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_GLFW_PLATFORM_API_H_
