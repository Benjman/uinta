#include "uinta/glfw_platform_api.h"

#include <cassert>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/gl.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"
#include "uinta/platform_events.h"
#include "uinta/status.h"
#include "uinta/types.h"
#include "uinta/window.h"

namespace uinta {

Status GlfwPlatformApi::initOpenGL() const noexcept {
  if (!glfwInit()) {
    return InternalError("Call to `glfwInit()` failed.");
  }
  return OkStatus();
}

StatusOr<std::vector<Monitor>> GlfwPlatformApi::findMonitors() const noexcept {
  i32 monitorCount;
  auto** monitorPtrs = glfwGetMonitors(&monitorCount);
  if (!monitorPtrs) return InternalError("Call to `glfwGetMonitors()` failed.");

  std::vector<Monitor> monitors;
  monitors.reserve(monitorCount);

  auto* primaryMonitor = glfwGetPrimaryMonitor();

  for (auto i = 0; i < monitorCount; i++) {
    auto* monitorPtr = monitorPtrs[i];
    auto* mode = glfwGetVideoMode(monitorPtr);
    auto* name = glfwGetMonitorName(monitorPtr);
    auto isPrimary = monitorPtr == primaryMonitor;
    auto& monitor =
        monitors.emplace_back(name, mode->width, mode->height,
                              mode->refreshRate, monitorPtr, isPrimary);
    LOG(INFO) << absl::StrFormat(
        "Found monitor %s %ix%i@%ihz%s", monitor.name(), monitor.width(),
        monitor.height(), monitor.hz(),
        monitor.flags().isPrimary() ? " (Primary)" : "");
  }

  glfwSetMonitorCallback([](auto* monitorPtr, const auto event) {
    // TODO: This needs to be tested. It should be triggered on a monitor
    // disconnect or connecting to the OS.
    if (auto* api = GlfwPlatformApi::Instance();
        api && api->windowData_.platform) {
      auto savedMonitor = api->windowData_.platform->findMonitor(monitorPtr);
      auto* primaryMonitor = glfwGetPrimaryMonitor();
      auto* mode = glfwGetVideoMode(monitorPtr);
      auto* name = glfwGetMonitorName(monitorPtr);
      auto isPrimary = monitorPtr == primaryMonitor;
      auto isConnected = event == GLFW_CONNECTED;
      auto* monitor =
          savedMonitor.value_or(api->windowData_.platform->addMonitor(
              {name, mode->width, mode->height, mode->refreshRate, monitorPtr,
               isPrimary, isConnected}));
      api->windowData_.platform->dispatch<PlatformEvent::OnMonitorChange>(
          OnMonitorChangeEvent(monitor));
    }
  });

  return monitors;
}

StatusOr<void*> GlfwPlatformApi::createWindow(Window* window) noexcept {
  constexpr auto versionMajor = 3;
  constexpr auto versionMinor = 3;
  LOG(INFO) << absl::StrFormat(
      "Initializing GLFW v%i.%i with OpenGL Core profile...", versionMajor,
      versionMinor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef UINTA_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwSetErrorCallback([](auto code, const auto* description) {
    auto* api = GlfwPlatformApi::Instance();
    if (api->windowData_.platform)
      api->windowData_.platform->dispatch<PlatformEvent::OnError>(
          OnErrorEvent(code, description));
  });

  GLFWmonitor* monitor = nullptr;

  if (window->isFullscreen() && window->monitor()) {
    monitor = static_cast<GLFWmonitor*>(window->monitor()->userData());
  }

  const auto name = window->name();
  i32 width = window->width();
  i32 height = window->height();

  if (auto* ptr =
          glfwCreateWindow(width, height, name.data(), monitor, nullptr);
      ptr) {
    glfwMakeContextCurrent(ptr);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      return InternalError("Call to `gladLoadGLLoader()` failed.");
    }

    glfwGetWindowSize(ptr, &width, &height);
    window->size(width, height);

    glfwSetWindowUserPointer(ptr, &windowData_);

    glfwSetWindowCloseCallback(ptr, [](auto* window) {
      auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
      data->platform->dispatch<PlatformEvent::OnCloseRequest>(
          OnCloseRequestEvent(window));
    });

    glfwSetFramebufferSizeCallback(ptr, [](auto* window, i32 width,
                                           i32 height) {
      auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
      auto* platform = data->platform;
      assert(platform &&
             "Platform was null when framebuffer size callback was invoked.");
      if (auto* platformWindow = platform->window()) {
        platformWindow->size(width, height);
      }
      platform->dispatch<PlatformEvent::OnViewportSizeChange>(
          OnViewportSizeChangeEvent(window, width, height));
    });

#ifdef UINTA_DEBUG
#ifdef GL_VERSION_4_3
    if (glfwExtensionSupported("GL_KHR_debug")) {
      glDebugMessageCallback(
          [](auto source, auto type, auto id, auto severity, auto length,
             const auto* message, const auto* userParam) {
            auto data = static_cast<const WindowData*>(userParam);
            auto* platform = data->platform;
            assert(
                platform &&
                "Platform was null when debug message callback was invoked.");
            platform->dispatch<PlatformEvent::OnDebugMessage>(
                OnDebugMessageEvent(source, type, id, severity, length,
                                    message));
          },
          &windowData_);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                            GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    }
#endif  // GL_VERSION_4_3
#endif  // UINTA_DEBUG

    return ptr;
  } else {
    return InternalError("Call to `glfwCreateWindow()` failed.");
  }
}

void GlfwPlatformApi::getWindowPos(const Window* window, i32* x,
                                   i32* y) const noexcept {
  if (window) {
    glfwGetWindowPos(static_cast<GLFWwindow*>(window->userData()), x, y);
  }
}

void GlfwPlatformApi::getWindowSize(const Window* window, i32* w,
                                    i32* h) const noexcept {
  if (window) {
    glfwGetWindowSize(static_cast<GLFWwindow*>(window->userData()), w, h);
  }
}

Status GlfwPlatformApi::pollEvents() const noexcept {
  glfwPollEvents();
  return OkStatus();
}

StatusOr<time_t> GlfwPlatformApi::runtime() const noexcept {
  return glfwGetTime();
}

Status GlfwPlatformApi::setWindowPosition(void* window, f32 x,
                                          f32 y) const noexcept {
  assert(window && "`void* window` cannot be null.");
  glfwSetWindowPos(static_cast<GLFWwindow*>(window), x, y);
  return OkStatus();
}

Status GlfwPlatformApi::swapBuffers(const Window* window) const noexcept {
  glfwSwapBuffers(static_cast<GLFWwindow*>(window->userData()));
  return OkStatus();
}

Status GlfwPlatformApi::destroy(const Window* window) const noexcept {
  if (window && window->userData()) {
    try {
      glfwDestroyWindow(static_cast<GLFWwindow*>(window->userData()));
      glfwTerminate();
    } catch (const std::exception& e) {
      return InternalError(e.what());
    }
  }
  return OkStatus();
}

}  // namespace uinta
