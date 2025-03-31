#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_PLATFORM_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_PLATFORM_H_

#include <functional>
#include <memory>

#include "uinta/args.h"
#include "uinta/mock/mock_runtime_getter.h"
#include "uinta/platform.h"
#include "uinta/status.h"
#include "uinta/window.h"

namespace uinta {

struct MockPlatform : Platform {
  ArgsProcessor args = ArgsProcessor(0, nullptr);

  explicit MockPlatform(Monitor* monitor = nullptr) noexcept {
    if (monitor)
      monitors_ = {*monitor};
    else
      monitors_ = {{"Test monitor", 1920, 1080, 144, nullptr, true}};
    window_ = std::make_unique<Window>(this);
  }

  std::function<void(i32*, i32*)> onGetAndUpdateWindowSize =
      [](i32*, i32*) -> void {};
  void getAndUpdateWindowSize(i32* w, i32* h) const noexcept override {
    onGetAndUpdateWindowSize(w, h);
  }

  std::function<Status()> onPollEvents = []() -> Status { return OkStatus(); };
  Status pollEvents() const noexcept override { return onPollEvents(); }

  MockRuntimeGetter runtimeGetter_;
  std::function<StatusOr<time_t>()> onRuntime = [this] {
    return runtimeGetter_.runtime();
  };
  StatusOr<time_t> runtime() const noexcept override { return onRuntime(); }

  std::function<Status()> onSwapBuffers = []() -> Status { return OkStatus(); };
  Status swapBuffers() const noexcept override { return onSwapBuffers(); }

  std::function<StatusOr<void*>(const Window*)> onCreateWindow =
      [](const Window*) -> StatusOr<void*> { return nullptr; };
  StatusOr<void*> createWindow(const Window* window) noexcept {
    return onCreateWindow(window);
  }

  std::function<Status(const Window*)> onDestroy = [](const Window*) -> Status {
    return OkStatus();
  };
  Status destroy(const Window* window) const noexcept {
    return onDestroy(window);
  }

  std::function<Status(void*, f32, f32)> onSetWindowPosition =
      [](void*, f32, f32) -> Status { return OkStatus(); };
  Status setWindowPosition(void* window, f32 x, f32 y) const noexcept {
    return onSetWindowPosition(window, x, y);
  }

  std::function<void(bool, const Window*, const Monitor*)> onMakeFullscreen =
      [](bool, const Window*, const Monitor*) {};
  void makeFullscreen(bool makeFullscreen, Window* window,
                      const Monitor* monitor) noexcept override {
    onMakeFullscreen(makeFullscreen, window, monitor);
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_PLATFORM_H_
