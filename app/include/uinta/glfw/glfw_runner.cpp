// clang-format off
#include <uinta/logging.hpp>
#include <spdlog/stopwatch.h>
// clang-format on

#include <uinta/error.hpp>
#include <uinta/gl/utils/type_utils.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/glfw/glfw_runner_ui.hpp>
#include <uinta/input.hpp>

namespace uinta {

enum class error {
  InitError = 100,
  WindowError = 200,
  GladError = 300,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::InitError), "Failed to initialize GLFW!"},
    {static_cast<uinta_error_code_t>(error::WindowError), "Failed to create GLFW window!"},
    {static_cast<uinta_error_code_t>(error::GladError), "Failed to load GLAD!"},
};

UINTA_ERROR_FRAMEWORK(GLFWRunner, errorMessages);

GlfwRunner::~GlfwRunner() {
  if (m_window) glfwDestroyWindow(m_window);
  glfwTerminate();
}

uinta_error_code GlfwRunner::doInit() {
  if (auto error = Runner::doInit(); error) return error;
  registerCallbacks();
  ui::onInit(*this);
  return SUCCESS_EC;
}

void GlfwRunner::pollInput() {
  glfwPollEvents();
  if (m_window && glfwWindowShouldClose(m_window)) setFlag(IS_RUNNING, false, m_flags);
}

f64 GlfwRunner::runtime() {
  return glfwGetTime();
}

void GlfwRunner::swapBuffers() {
  glfwSwapBuffers(m_window);
}

uinta_error_code GlfwRunner::createOpenGLContext() {
  spdlog::stopwatch sw;

  constexpr i32 version_major = 3;
  constexpr i32 version_minor = 3;
  SPDLOG_LOGGER_INFO(m_logger, "Initializing GLFW v{}.{} with OpenGL Core profile...", version_major, version_minor);
  if (!glfwInit()) return make_error(error::InitError);

  i32 monCount;
  auto** const mons = glfwGetMonitors(&monCount);
  for (i32 i = 0; i < std::min(monCount, GlfwRunner::MAX_MONITORS); ++i) {
    auto* const mon = mons[i];
    auto* const view = glfwGetVideoMode(mon);
    m_monitors[i] = {mon, glfwGetMonitorName(mon), view->width, view->height, view->refreshRate};
  }
  // TODO: We need to listen for monitor changes, and update the monitor list. See
  // https://www.glfw.org/docs/3.3/group__monitor.html#gab39df645587c8518192aa746c2fb06c3
  // `GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback)`

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef UINTA_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  auto* const target_mon = window().fullscreen ? m_monitors[0].ptr : NULL;
  const auto target_width = window().width ? window().width : m_monitors[0].width;
  const auto target_height = window().height ? window().height : m_monitors[0].height;
  m_window = glfwCreateWindow(target_width, target_height, window().title.c_str(), target_mon, NULL);
  if (!m_window) return make_error(error::WindowError);

  const auto* const view = glfwGetVideoMode(m_monitors[0].ptr);
  const auto target_x = view ? view->width / 2.0 - target_width / 2.0 : 0;
  const auto target_y = view ? view->height / 2.0 - target_height / 2.0 : 0;
  glfwSetWindowPos(m_window, target_x, target_y);

  window(Window(window().title, target_width, target_height));
  SPDLOG_LOGGER_INFO(m_logger, "Created window '{}' {}x{} (aspect ratio {}).", window().title, target_width, target_height,
                     window().aspect_ratio);

  glfwSetWindowUserPointer(m_window, this);
  glfwMakeContextCurrent(m_window);

  SPDLOG_LOGGER_INFO(m_logger, "Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return make_error(error::GladError);

  SPDLOG_LOGGER_INFO(m_logger, "GLFW initialization completed in {} seconds.", sw.elapsed().count());

  return SUCCESS_EC;
}

void GlfwRunner::doPreTick() {
  ui::onPreTick(*this, state());
  Runner::doPreTick();
}

void GlfwRunner::doTick() {
  Runner::doTick();
  ui::onTick(*this, state());
}

void GlfwRunner::doPostTick() {
  Runner::doPostTick();
  ui::onPostTick(*this, state());
}

void GlfwRunner::doPreRender() {
  ui::onPreRender(*this, state());
  Runner::doPreRender();
}

void GlfwRunner::doRender() {
  Runner::doRender();
  ui::onRender(*this, state());
}

void GlfwRunner::doPostRender() {
  Runner::doPostRender();
  if (isFlagSet(ui::INPUT_HANDLED_KEYBOARD, ui::flags)) resetKeyboard(input());
  if (isFlagSet(ui::INPUT_HANDLED_MOUSE, ui::flags)) resetMouse(input());
  ui::flags = 0;
  ui::onPostRender(*this, state());
}

void GlfwRunner::doShutdown() {
  Runner::doShutdown();
  ui::onShutdown(*this);
}

void GlfwRunner::registerCallbacks() {
  if (!m_window) return;

  glfwSetKeyCallback(m_window, [](auto* window, i32 key, i32 scancode, i32 action, i32 mods) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) {
      if (mods & GLFW_MOD_CONTROL) {
        const size_t exit_code = 1;
        SPDLOG_LOGGER_CRITICAL(const_cast<spdlog::logger*>(runner->logger()),
                               "Intentionally hard exiting the application with exit code {}.", exit_code);
        exit(exit_code);
      }
      auto flags = runner->flags();
      setFlag(IS_RUNNING, false, flags);
      runner->flags(flags);
    }
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(m_window, [](auto* m_window, f64 xpos, f64 ypos) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(m_window, [](auto* m_window, i32 button, i32 action, i32 mods) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(m_window, [](auto* m_window, f64 xoffset, f64 yoffset) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(m_window, [](auto* m_window, i32 width, i32 height) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(m_window, [](auto* m_window, i32 xpos, i32 ypos) {
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleWindowPosChanged(xpos, ypos);
  });

#ifdef UINTA_DEBUG
#ifdef GL_VERSION_4_3
  if (glfwExtensionSupported("GL_KHR_debug")) {
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                              const void* userParam) { SPDLOG_CRITICAL("OpenGL ERROR: [{}]: {}", severity, message); },
                           nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
  }
#endif
#endif
}

void GlfwRunner::setWindowPosition(u32 x, u32 y) {
  glfwSetWindowPos(m_window, x, y);
}

}  // namespace uinta
