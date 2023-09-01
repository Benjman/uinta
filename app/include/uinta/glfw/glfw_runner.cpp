// clang-format off
#include <uinta/logging.hpp>
#include <spdlog/stopwatch.h>
// clang-format on

#include <uinta/error.hpp>
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
  if (glfwWindow) glfwDestroyWindow(glfwWindow);
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
  if (glfwWindow && glfwWindowShouldClose(glfwWindow)) setFlag(SHUTDOWN, true, flags);
}

f64 GlfwRunner::getRuntime() const {
  return glfwGetTime();
}

void GlfwRunner::swapBuffers() {
  glfwSwapBuffers(glfwWindow);
}

uinta_error_code GlfwRunner::createOpenGLContext() {
  spdlog::stopwatch sw;

  constexpr i32 version_major = 3;
  constexpr i32 version_minor = 3;
  SPDLOG_INFO("Initializing GLFW v{}.{} with OpenGL Core profile...", version_major, version_minor);
  if (!glfwInit()) return make_error(error::InitError);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  glfwWindow = glfwCreateWindow(window.width, window.height, window.title.c_str(), NULL, NULL);
  if (!glfwWindow) return make_error(error::WindowError);
  SPDLOG_INFO("Created glfwWindow '{}' {}x{} (aspect ratio {}).", window.title, window.width, window.height, window.aspectRatio);

  glfwSetWindowUserPointer(glfwWindow, this);
  glfwMakeContextCurrent(glfwWindow);

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
  if (isFlagSet(ui::INPUT_HANDLED_KEYBOARD, ui::flags)) resetKeyboard(input);
  if (isFlagSet(ui::INPUT_HANDLED_MOUSE, ui::flags)) resetMouse(input);
  ui::flags = 0;
  /* SPDLOG_CRITICAL(v); */
  ui::onPostRender(*this, state);
}

void GlfwRunner::doShutdown() {
  Runner::doShutdown();
  ui::onShutdown(*this);
}

void GlfwRunner::registerCallbacks() {
  if (!glfwWindow) return;

  glfwSetKeyCallback(glfwWindow, [](auto* glfwWindow, i32 key, i32 scancode, i32 action, i32 mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(glfwWindow));
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return setFlag(Runner::SHUTDOWN, true, runner->flags);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(glfwWindow, [](auto* glfwWindow, f64 xpos, f64 ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypos);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(glfwWindow));
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(glfwWindow, [](auto* glfwWindow, i32 button, i32 action, i32 mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(glfwWindow));
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(glfwWindow, [](auto* glfwWindow, f64 xoffset, f64 yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(glfwWindow));
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(glfwWindow, [](auto* glfwWindow, i32 width, i32 height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    auto* const runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(glfwWindow));
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(glfwWindow, [](auto* glfwWindow, i32 xpos, i32 ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    glfwGetWindowUserPointer(glfwWindow);
  });
}

}  // namespace uinta
