#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_DESKTOP_PLATFORM_API_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_DESKTOP_PLATFORM_API_H_

#include <absl/status/status.h>

#include <functional>
#include <vector>

#include "uinta/desktop_platform_api.h"
#include "uinta/monitor.h"
#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

struct MockDesktopPlatformApi : DesktopPlatformApi {
  std::function<StatusOr<void*>(Window*)> onCreateWindow =
      [](Window*) -> StatusOr<void*> { return nullptr; };
  StatusOr<void*> createWindow(Window* window) noexcept override {
    return onCreateWindow(window);
  }

  std::function<Status(const Window*)> onDestroy = [](const Window*) -> Status {
    return OkStatus();
  };
  Status destroy(const Window* window) const noexcept override {
    return onDestroy(window);
  }

  std::function<StatusOr<std::vector<Monitor>>()> onFindMonitors = []() {
    return std::vector<Monitor>{
        {"Test monitor", 1920, 1080, 144, nullptr, true}};
  };
  StatusOr<std::vector<Monitor>> findMonitors() const noexcept override {
    return onFindMonitors();
  }

  std::function<void(const Window*, i32*, i32*)> onGetWindowPos =
      [](const Window*, i32* x, i32* y) {
        *x = 0;
        *y = 0;
      };
  void getWindowPos(const Window* window, i32* x,
                    i32* y) const noexcept override {
    onGetWindowPos(window, x, y);
  }

  std::function<void(const Window*, i32*, i32*)> onGetWindowSize =
      [](const Window*, i32* w, i32* h) {
        *w = 800;
        *h = 600;
      };
  void getWindowSize(const Window* window, i32* w,
                     i32* h) const noexcept override {
    onGetWindowSize(window, w, h);
  }

  std::function<Status()> onInitOpenGL = []() -> Status { return OkStatus(); };
  Status initOpenGL() const noexcept override { return onInitOpenGL(); }

  std::function<Status()> onPollEvents = []() -> Status { return OkStatus(); };
  Status pollEvents() const noexcept override { return onPollEvents(); }

  mutable time_t runtime_ = 0;
  static constexpr time_t RuntimeStep = 1.0 / 144.0;
  std::function<StatusOr<time_t>()> onRuntime = [this]() -> StatusOr<time_t> {
    runtime_ += RuntimeStep;
    return runtime_;
  };
  StatusOr<time_t> runtime() const noexcept override { return onRuntime(); }

  std::function<Status(void*, u32, u32)> onSetWindowPosition =
      [](void*, u32, u32) -> Status { return OkStatus(); };
  Status setWindowPosition(void* window, u32 x, u32 y) const noexcept override {
    return onSetWindowPosition(window, x, y);
  }

  std::function<Status(const Window*)> onSwapBuffers =
      [](const Window*) -> Status { return OkStatus(); };
  Status swapBuffers(const Window* window) const noexcept override {
    return onSwapBuffers(window);
  }

  std::function<Status(const Window*, Input*)> onRegisterInputHandlers =
      [](const Window*, Input*) -> Status { return OkStatus(); };
  Status registerInputHandlers(const Window* window,
                               Input* input) noexcept override {
    return onRegisterInputHandlers(window, input);
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_DESKTOP_PLATFORM_API_H_
