#ifndef SRC_ENGINE_TEST_SRC_MOCK_MOCK_PLATFORM_H_
#define SRC_ENGINE_TEST_SRC_MOCK_MOCK_PLATFORM_H_

#include <functional>
#include <memory>

#include "./mock_runtime_getter.h"
#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/window.h"

namespace uinta {

class MockPlatform : public Platform {
 public:
  explicit MockPlatform(Monitor* monitor = nullptr) noexcept
      : Platform(nullptr) {
    WindowConfig config("Test window", 0, 0, 1920, 1080);
    if (monitor)
      monitors_ = {*monitor};
    else
      monitors_ = {{"Test monitor", 1920, 1080, 144, nullptr, true}};
    window_ = std::make_unique<Window>(config, this);
  }

  ~MockPlatform() noexcept override {}
  MockPlatform(const MockPlatform&) noexcept = delete;
  MockPlatform& operator=(const MockPlatform&) noexcept = delete;
  MockPlatform(MockPlatform&&) noexcept = delete;
  MockPlatform& operator=(MockPlatform&&) noexcept = delete;

  std::function<void(i32*, i32*)> onGetAndUpdateWindowSize =
      [](i32*, i32*) -> void {};
  void getAndUpdateWindowSize(i32* w, i32* h) const noexcept override {
    onGetAndUpdateWindowSize(w, h);
  }

  std::function<Status()> onPollEvents = []() -> Status { return OkStatus(); };
  Status pollEvents() const noexcept override { return onPollEvents(); }

  MockRuntimeGetter runtimeGetter_;
  std::function<StatusOr<time_t>()> onRuntime = [this] -> StatusOr<time_t> {
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

  std::function<Status(Input*)> onRegisterInputHandlers = [](Input*) -> Status {
    return OkStatus();
  };
  Status registerInputHandlers(Input* input) const noexcept override {
    return onRegisterInputHandlers(input);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_MOCK_MOCK_PLATFORM_H_
