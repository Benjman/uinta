// clang-format off
#include <uinta/logging.hpp>
#include <spdlog/stopwatch.h>
// clang-format on

#include <uinta/glfw_utils/glfw_runner.hpp>
#include <uinta/input/utils.hpp>

namespace uinta {

GlfwRunner::~GlfwRunner() {
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

bool GlfwRunner::doInit() {
  Runner::doInit();
  registerCallbacks(this);
  ui.onInit(*this);
  return true;
}

void GlfwRunner::pollInput() {
  glfwPollEvents();
}

double GlfwRunner::getRuntime() const {
  return glfwGetTime();
}

void GlfwRunner::swapBuffers() {
  glfwSwapBuffers(window);
}

bool GlfwRunner::createOpenGLContext() {
  if (!createGLFWWindow(this)) {
    SPDLOG_ERROR("Failed to create OpenGL context!");
    return false;
  }
  return true;
}

bool GlfwRunner::shouldExit() {
  return window && glfwWindowShouldClose(window);
}

void GlfwRunner::doPreTick(const RunnerState& state) {
  Runner::doPreTick(state);
  ui.onPreTick(*this);
}

void GlfwRunner::doTick(const RunnerState& state) {
  Runner::doTick(state);
  ui.onTick(*this);
}

void GlfwRunner::doPostTick(const RunnerState& state) {
  Runner::doPostTick(state);
  ui.onPostTick(*this);
}

void GlfwRunner::doPreRender(const RunnerState& state) {
  Runner::doPreRender(state);
  ui.onPreRender(*this);
}

void GlfwRunner::doRender(const RunnerState& state) {
  Runner::doRender(state);
  ui.onRender(*this);
}

void GlfwRunner::doPostRender(const RunnerState& state) {
  Runner::doPostRender(state);
  ui.onPostRender(*this);
  flags_t flags = ui.flags;
  if (isFlagSet(INPUT_HANDLED_KEYBOARD, flags)) resetKeyboard(input);
  if (isFlagSet(INPUT_HANDLED_MOUSE, flags)) resetMouse(input);
  ui.flags = 0;
}

void GlfwRunner::doShutdown() {
  Runner::doShutdown();
  ui.onShutdown(*this);
}

bool createGLFWWindow(GlfwRunner* runner) {
  spdlog::stopwatch sw;

  int version_major = 3;
  int version_minor = 3;

  SPDLOG_INFO("Initializing GLFW v{}.{} with OpenGL Core profile...", version_major, version_minor);

  if (!glfwInit()) {
    SPDLOG_ERROR("Failed to initialize GLFW!");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  runner->window = glfwCreateWindow(runner->display.width, runner->display.height, runner->display.title.c_str(), NULL, NULL);
  if (!runner->window) {
    SPDLOG_ERROR("Failed to create GLFW window!");
    return false;
  }

  SPDLOG_INFO("Created window '{}' {}x{} (aspect ratio {}).", runner->display.title, runner->display.width,
              runner->display.height, runner->display.aspectRatio);

  glfwSetWindowUserPointer(runner->window, runner);
  glfwMakeContextCurrent(runner->window);

  SPDLOG_INFO("Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SPDLOG_ERROR("Failed to load GLAD!");
    return false;
  }

  SPDLOG_INFO("GLFW initialization completed in {} seconds.", sw.elapsed().count());

  return true;
}

inline void registerCallbacks(GlfwRunner* runner) {
  if (!runner->window) return;

  glfwSetKeyCallback(runner->window, [](auto* window, int key, int scancode, int action, int mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return glfwSetWindowShouldClose(runner->window, true);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(runner->window, [](auto* window, double xpos, double ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypox);
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(runner->window, [](auto* window, int button, int action, int mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(runner->window, [](auto* window, double xoffset, double yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(runner->window, [](auto* window, int width, int height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(runner->window, [](auto* window, int xpos, int ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    auto* runner = static_cast<GlfwRunner*>(glfwGetWindowUserPointer(window));
  });
}

}  // namespace uinta
