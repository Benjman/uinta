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
  if (m_window && glfwWindowShouldClose(m_window)) flag(STOP_RUNNING, true);
}

f64 GlfwRunner::runtime() const {
  return glfwGetTime();
}

void GlfwRunner::swapBuffers() {
  glfwSwapBuffers(m_window);
}

uinta_error_code GlfwRunner::createOpenGLContext() {
  spdlog::stopwatch sw;

  constexpr i32 version_major = 3;
  constexpr i32 version_minor = 3;
  SPDLOG_INFO("Initializing GLFW v{}.{} with OpenGL Core profile...", version_major, version_minor);
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

  u32 targetWidth = window().width ? window().width : m_monitors[0].width;
  u32 targetHeight = window().height ? window().height : m_monitors[0].height;
  GLFWmonitor* targetMon = isFlagSet(Window::FULLSCREEN, window().flags) ? m_monitors[0].ptr : NULL;
  m_window = glfwCreateWindow(targetWidth, targetHeight, window().title.c_str(), targetMon, NULL);
  if (!m_window) return make_error(error::WindowError);
  SPDLOG_INFO("Created window '{}' {}x{} (aspect ratio {}).", window().title, targetWidth, targetHeight, window().aspectRatio);
  window(Window(window().title, targetWidth, targetHeight));

  glfwSetWindowUserPointer(m_window, this);
  glfwMakeContextCurrent(m_window);

  SPDLOG_INFO("Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return make_error(error::GladError);

  SPDLOG_INFO("GLFW initialization completed in {} seconds.", sw.elapsed().count());

  return SUCCESS_EC;
}

void GlfwRunner::doPreTick(const RunnerState& state) {
  ui::onPreTick(*this, state);
  Runner::doPreTick(state);
}

void GlfwRunner::doTick(const RunnerState& state) {
  Runner::doTick(state);
  ui::onTick(*this, state);
}

void GlfwRunner::doPostTick(const RunnerState& state) {
  Runner::doPostTick(state);
  ui::onPostTick(*this, state);
}

void GlfwRunner::doPreRender(const RunnerState& state) {
  ui::onPreRender(*this, state);
  Runner::doPreRender(state);
}

void GlfwRunner::doRender(const RunnerState& state) {
  Runner::doRender(state);
  ui::onRender(*this, state);
}

void GlfwRunner::doPostRender(const RunnerState& state) {
  Runner::doPostRender(state);
  if (isFlagSet(ui::INPUT_HANDLED_KEYBOARD, ui::flags)) resetKeyboard(input());
  if (isFlagSet(ui::INPUT_HANDLED_MOUSE, ui::flags)) resetMouse(input());
  ui::flags = 0;
  /* SPDLOG_CRITICAL(v); */
  ui::onPostRender(*this, state);
}

void GlfwRunner::doShutdown() {
  Runner::doShutdown();
  ui::onShutdown(*this);
}

void GlfwRunner::registerCallbacks() {
  if (!m_window) return;

  glfwSetKeyCallback(m_window, [](auto* window, i32 key, i32 scancode, i32 action, i32 mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) runner->flag(STOP_RUNNING, true);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(m_window, [](auto* m_window, f64 xpos, f64 ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypos);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(m_window, [](auto* m_window, i32 button, i32 action, i32 mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(m_window, [](auto* m_window, f64 xoffset, f64 yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(m_window, [](auto* m_window, i32 width, i32 height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(m_window));
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(m_window, [](auto* m_window, i32 xpos, i32 ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    glfwGetWindowUserPointer(m_window);
  });

#ifdef UINTA_DEBUG
  if (glfwExtensionSupported("GL_KHR_debug")) {
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                              const void* userParam) { SPDLOG_CRITICAL("OpenGL ERROR: [{}]: {}", severity, message); },
                           nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
  }
#endif
}

}  // namespace uinta
