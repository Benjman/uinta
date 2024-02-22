#include "uinta/glfw_platform_api.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/input.h"
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
  if (!glfwInit()) {
    return InternalError("Call to `glfwInit()` failed.");
  }
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
              source, type, id, severity, length, message);
        },
        &windowData_);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
  }
#endif  // GL_VERSION_4_3
#endif  // UINTA_DEBUG

  return ptr;
}

void GlfwPlatformApi::getWindowSize(const Window* window, i32* w,
                                    i32* h) const noexcept {
  glfwGetWindowSize(static_cast<GLFWwindow*>(window->userData()), w, h);
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
  if (window && window->userData()) {
    try {
      glfwDestroyWindow(static_cast<GLFWwindow*>(window->userData()));
      glfwTerminate();
    } catch (const std::exception& e) {
      return InternalError(e.what());
    }
  }
  return {};
}

Status GlfwPlatformApi::registerInputHandlers(const Window* window,
                                              Input* input) noexcept {
  assert(input && "Input* cannot be null.");
  if (windowData_.input)
    return CancelledError(
        "Unexpected multiple call to "
        "`GlfwPlatformApi::registerInputHandlers()`.");

  windowData_.input = input;

  assert(window && "Window* cannot be null.");
  auto glfwWindow = static_cast<GLFWwindow*>(window->userData());

  glfwSetKeyCallback(glfwWindow, [](auto* window, i32 key, i32 scancode,
                                    i32 action, i32 mods) {
    auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    KeyboardEvent event(window, key, scancode, action, mods);
    if (auto status = data->input->onKey(event); !status.ok())
      LOG(WARNING) << status.message();
  });

  glfwSetCursorPosCallback(glfwWindow, [](auto* window, f64 x, f64 y) {
    auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    MouseEvent event(window, x, y, -1, -1, 0);
    if (auto status = data->input->onCursorPos(event); !status.ok())
      LOG(WARNING) << status.message();
  });

  glfwSetScrollCallback(glfwWindow, [](auto* window, f64 x, f64 y) {
    auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    MouseEvent event(window, x, y, _::InputUnknown, _::InputUnknown, 0);
    if (auto status = data->input->onScroll(event); !status.ok())
      LOG(WARNING) << status.message();
  });

  glfwSetMouseButtonCallback(
      glfwWindow, [](auto* window, i32 button, i32 action, i32 mods) {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseEvent event(window, _::InputUnknown, _::InputUnknown, button,
                         action, mods);
        if (auto status = data->input->onMouseButton(event); !status.ok())
          LOG(WARNING) << status.message();
      });

  return {};
}

InputKey ACTION_PRESS = GLFW_PRESS;
InputKey ACTION_RELEASE = GLFW_RELEASE;
InputKey ACTION_REPEAT = GLFW_REPEAT;

InputKey MOD_ALT = GLFW_MOD_ALT;
InputKey MOD_CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
InputKey MOD_CONTROL = GLFW_MOD_CONTROL;
InputKey MOD_NUM_LOCK = GLFW_MOD_NUM_LOCK;
InputKey MOD_SHIFT = GLFW_MOD_SHIFT;
InputKey MOD_SUPER = GLFW_MOD_SUPER;

InputKey KEY_0 = GLFW_KEY_0;
InputKey KEY_1 = GLFW_KEY_1;
InputKey KEY_2 = GLFW_KEY_2;
InputKey KEY_3 = GLFW_KEY_3;
InputKey KEY_4 = GLFW_KEY_4;
InputKey KEY_5 = GLFW_KEY_5;
InputKey KEY_6 = GLFW_KEY_6;
InputKey KEY_7 = GLFW_KEY_7;
InputKey KEY_8 = GLFW_KEY_8;
InputKey KEY_9 = GLFW_KEY_9;
InputKey KEY_A = GLFW_KEY_A;
InputKey KEY_B = GLFW_KEY_B;
InputKey KEY_C = GLFW_KEY_C;
InputKey KEY_D = GLFW_KEY_D;
InputKey KEY_E = GLFW_KEY_E;
InputKey KEY_F = GLFW_KEY_F;
InputKey KEY_G = GLFW_KEY_G;
InputKey KEY_H = GLFW_KEY_H;
InputKey KEY_I = GLFW_KEY_I;
InputKey KEY_J = GLFW_KEY_J;
InputKey KEY_K = GLFW_KEY_K;
InputKey KEY_L = GLFW_KEY_L;
InputKey KEY_M = GLFW_KEY_M;
InputKey KEY_N = GLFW_KEY_N;
InputKey KEY_O = GLFW_KEY_O;
InputKey KEY_P = GLFW_KEY_P;
InputKey KEY_Q = GLFW_KEY_Q;
InputKey KEY_R = GLFW_KEY_R;
InputKey KEY_S = GLFW_KEY_S;
InputKey KEY_T = GLFW_KEY_T;
InputKey KEY_U = GLFW_KEY_U;
InputKey KEY_V = GLFW_KEY_V;
InputKey KEY_W = GLFW_KEY_W;
InputKey KEY_X = GLFW_KEY_X;
InputKey KEY_Y = GLFW_KEY_Y;
InputKey KEY_Z = GLFW_KEY_Z;

InputKey KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
InputKey KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
InputKey KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
InputKey KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
InputKey KEY_COMMA = GLFW_KEY_COMMA;
InputKey KEY_DELETE = GLFW_KEY_DELETE;
InputKey KEY_DOWN = GLFW_KEY_DOWN;
InputKey KEY_END = GLFW_KEY_END;
InputKey KEY_ENTER = GLFW_KEY_ENTER;
InputKey KEY_EQUAL = GLFW_KEY_EQUAL;
InputKey KEY_ESCAPE = GLFW_KEY_ESCAPE;
InputKey KEY_F1 = GLFW_KEY_F1;
InputKey KEY_F10 = GLFW_KEY_F10;
InputKey KEY_F11 = GLFW_KEY_F11;
InputKey KEY_F12 = GLFW_KEY_F12;
InputKey KEY_F13 = GLFW_KEY_F13;
InputKey KEY_F14 = GLFW_KEY_F14;
InputKey KEY_F15 = GLFW_KEY_F15;
InputKey KEY_F16 = GLFW_KEY_F16;
InputKey KEY_F17 = GLFW_KEY_F17;
InputKey KEY_F18 = GLFW_KEY_F18;
InputKey KEY_F19 = GLFW_KEY_F19;
InputKey KEY_F2 = GLFW_KEY_F2;
InputKey KEY_F20 = GLFW_KEY_F20;
InputKey KEY_F21 = GLFW_KEY_F21;
InputKey KEY_F22 = GLFW_KEY_F22;
InputKey KEY_F23 = GLFW_KEY_F23;
InputKey KEY_F24 = GLFW_KEY_F24;
InputKey KEY_F25 = GLFW_KEY_F25;
InputKey KEY_F3 = GLFW_KEY_F3;
InputKey KEY_F4 = GLFW_KEY_F4;
InputKey KEY_F5 = GLFW_KEY_F5;
InputKey KEY_F6 = GLFW_KEY_F6;
InputKey KEY_F7 = GLFW_KEY_F7;
InputKey KEY_F8 = GLFW_KEY_F8;
InputKey KEY_F9 = GLFW_KEY_F9;
InputKey KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
InputKey KEY_HOME = GLFW_KEY_HOME;
InputKey KEY_INSERT = GLFW_KEY_INSERT;
InputKey KEY_KP_0 = GLFW_KEY_KP_0;
InputKey KEY_KP_1 = GLFW_KEY_KP_1;
InputKey KEY_KP_2 = GLFW_KEY_KP_2;
InputKey KEY_KP_3 = GLFW_KEY_KP_3;
InputKey KEY_KP_4 = GLFW_KEY_KP_4;
InputKey KEY_KP_5 = GLFW_KEY_KP_5;
InputKey KEY_KP_6 = GLFW_KEY_KP_6;
InputKey KEY_KP_7 = GLFW_KEY_KP_7;
InputKey KEY_KP_8 = GLFW_KEY_KP_8;
InputKey KEY_KP_9 = GLFW_KEY_KP_9;
InputKey KEY_KP_ADD = GLFW_KEY_KP_ADD;
InputKey KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
InputKey KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
InputKey KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
InputKey KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
InputKey KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
InputKey KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
InputKey KEY_LEFT = GLFW_KEY_LEFT;
InputKey KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
InputKey KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
InputKey KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
InputKey KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
InputKey KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
InputKey KEY_MENU = GLFW_KEY_MENU;
InputKey KEY_MINUS = GLFW_KEY_MINUS;
InputKey KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
InputKey KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
InputKey KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
InputKey KEY_PAUSE = GLFW_KEY_PAUSE;
InputKey KEY_PERIOD = GLFW_KEY_PERIOD;
InputKey KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
InputKey KEY_RIGHT = GLFW_KEY_RIGHT;
InputKey KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
InputKey KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
InputKey KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
InputKey KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
InputKey KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
InputKey KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
InputKey KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
InputKey KEY_SLASH = GLFW_KEY_SLASH;
InputKey KEY_SPACE = GLFW_KEY_SPACE;
InputKey KEY_TAB = GLFW_KEY_TAB;
InputKey KEY_UP = GLFW_KEY_UP;
InputKey KEY_WORLD_1 = GLFW_KEY_WORLD_1;
InputKey KEY_WORLD_2 = GLFW_KEY_WORLD_2;

MouseButton MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
MouseButton MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
MouseButton MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
MouseButton MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
MouseButton MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
MouseButton MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
MouseButton MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
MouseButton MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;
MouseButton MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST;
MouseButton MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
MouseButton MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
MouseButton MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;

}  // namespace uinta
