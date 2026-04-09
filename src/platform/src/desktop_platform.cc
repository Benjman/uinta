#include "uinta/desktop_platform.h"

#include <GLFW/glfw3.h>
#include <absl/log/log.h>

#include <memory>

#include "uinta/desktop_window.h"
#include "uinta/glfw_platform_api.h"
#include "uinta/status.h"

namespace uinta {

DesktopPlatform::DesktopPlatform(AppConfig* appConfig,
                                 DesktopPlatformApi* api) noexcept
    : api_(api) {
  if (auto* casted = dynamic_cast<GlfwPlatformApi*>(api_)) {
    casted->platform(this);
  }

  if (status_ = api_->initOpenGL(); !status_.ok()) {
    return;
  }

  if (auto status = api_->findMonitors(); status.ok()) {
    monitors_ = status.value();
  } else {
    status_ = status.status();
    return;
  }

  if (window_ = std::make_unique<DesktopWindow>(this, appConfig);
      window_->status().ok()) {
    LOG(INFO) << "Initialized window.";
  } else {
    status_ = window_->status();
    return;
  }

  if (window_->isFullscreen()) {
    i32 w;
    i32 h;
    getAndUpdateWindowSize(&w, &h);
  }
}

void DesktopPlatform::makeFullscreen(bool makeFullscreen, Window* window,
                                     const Monitor* monitor) noexcept {
  assert(window && "Window* cannot be null.");

  auto* glfwWindow = static_cast<GLFWwindow*>(window->userData());
  GLFWmonitor* glfwMonitor = nullptr;

  if (makeFullscreen) {
    if (monitor != nullptr) {
      glfwMonitor = static_cast<GLFWmonitor*>(monitor->userData());
    } else {
      glfwMonitor =
          static_cast<GLFWmonitor*>(primaryMonitor().value()->userData());
    }
  } else {
  }

  glfwSetWindowMonitor(glfwWindow, glfwMonitor, static_cast<i32>(window->x()),
                       static_cast<i32>(window->y()),
                       static_cast<i32>(window->width()),
                       static_cast<i32>(window->height()),
                       (monitor != nullptr) ? monitor->hz() : 0);

  window->isFullscreen(makeFullscreen);
}

}  // namespace uinta
