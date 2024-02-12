#include "uinta/glfw_platform_api.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/desktop_platform.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/monitor.h"
#include "uinta/types.h"
#include "uinta/window.h"

namespace uinta {

void GlfwPlatformApi::onCloseRequest(const CloseRequestFunc& func) noexcept {
  if (!func) LOG(ERROR) << "onCloseRequest callback invalid.";
  windowData_.onCloseRequest = func;
}

void GlfwPlatformApi::onDebugMessage(const DebugMessageFunc& func) noexcept {
  if (!func) LOG(ERROR) << "onDebugMessage callback invalid.";
  windowData_.onDebugMessage = func;
}

void GlfwPlatformApi::onError(const ErrorFunc& func) noexcept {
  if (!func) LOG(ERROR) << "onError callback invalid.";
  windowData_.onError = func;
}

void GlfwPlatformApi::onFramebufferSize(
    const FramebufferSizeFunc& func) noexcept {
  if (!func) LOG(ERROR) << "onFramebufferSize callback invalid.";
  windowData_.onFramebufferSize = func;
}

Status GlfwPlatformApi::initOpenGL() const noexcept {
  if (!glfwInit()) return InternalError("Call to `glfwInit()` failed.");
  return {};
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
    auto name = glfwGetMonitorName(monitorPtr);
    auto isPrimary = monitorPtr == primaryMonitor;
    auto& monitor =
        monitors.emplace_back(name, mode->width, mode->height,
                              mode->refreshRate, monitorPtr, isPrimary);
    LOG(INFO) << StrFormat("Found monitor %s %ix%i@%ihz%s", monitor.name(),
                           monitor.width(), monitor.height(), monitor.hz(),
                           monitor.isPrimary() ? " (Primary)" : "");
  }

  return monitors;
}

Status GlfwPlatformApi::clearFrontBuffer(f32 r, f32 g, f32 b, f32 a,
                                         bitflag clearMask) const noexcept {
  glClearColor(r, g, b, a);
  glClear(clearMask);
  return {};
}

StatusOr<void*> GlfwPlatformApi::createWindow(const Window* window) noexcept {
  constexpr auto versionMajor = 3;
  constexpr auto versionMinor = 3;
  LOG(INFO) << StrFormat("Initializing GLFW v%i.%i with OpenGL Core profile...",
                         versionMajor, versionMinor);
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
    if (GlfwPlatformApi::GetInstance()->windowData_.onError)
      GlfwPlatformApi::GetInstance()->windowData_.onError(code, description);
    else
      LOG(ERROR) << description;
  });

  auto* ptr = glfwCreateWindow(
      window->width(), window->height(), window->name().data(),
      static_cast<GLFWmonitor*>(
          window->monitor() ? window->monitor()->userData() : nullptr),
      nullptr);
  if (!ptr) return InternalError("Call to `glfwCreateWindow()` failed.");

  glfwMakeContextCurrent(ptr);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    return InternalError("Call to `gladLoadGLLoader()` failed.");

  glfwSetWindowUserPointer(ptr, &windowData_);

  glfwSetWindowCloseCallback(ptr, [](auto* window) {
    auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    data->onCloseRequest(window);
  });

  glfwSetFramebufferSizeCallback(ptr, [](auto* window, i32 width, i32 height) {
    auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (data->onFramebufferSize) data->onFramebufferSize(width, height);
  });

#ifdef UINTA_DEBUG
#ifdef GL_VERSION_4_3
  if (glfwExtensionSupported("GL_KHR_debug")) {
    glDebugMessageCallback(
        [](auto source, auto type, auto id, auto severity, auto length,
           const auto* message, const auto* userParam) {
          static_cast<const WindowData*>(userParam)->onDebugMessage(
              source, type, id, severity, length, message, userParam);
        },
        &windowData_);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
  }
#endif  // GL_VERSION_4_3
#endif  // UINTA_DEBUG

  return ptr;
}

Status GlfwPlatformApi::pollEvents() const noexcept {
  glfwPollEvents();
  return {};
}

StatusOr<f64> GlfwPlatformApi::runtime() const noexcept {
  return glfwGetTime();
}

Status GlfwPlatformApi::setWindowPosition(void* window, f32 x,
                                          f32 y) const noexcept {
  assert(window && "`void* window` cannot be null.");
  glfwSetWindowPos(static_cast<GLFWwindow*>(window), x, y);
  return {};
}

Status GlfwPlatformApi::swapBuffers(const Window* window) const noexcept {
  glfwSwapBuffers(static_cast<GLFWwindow*>(window->userData()));
  return {};
}

Status GlfwPlatformApi::destroy(const Window* window) const noexcept {
  assert(window && "`Window*` cannot be null.");

  if (window->userData()) {
    try {
      glfwDestroyWindow(static_cast<GLFWwindow*>(window->userData()));
      glfwTerminate();
    } catch (const std::exception& e) {
      return InternalError(e.what());
    }
  }
  return {};
}

}  // namespace uinta
