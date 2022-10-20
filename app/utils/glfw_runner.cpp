// clang-format off
#include <uinta/logging.hpp>
#include <spdlog/stopwatch.h>
// clang-format on

#include "./glfw_runner.hpp"

#include <uinta/gl/api.hpp>
#include <uinta/input/utils.hpp>

namespace uinta {

GlfwRunner::~GlfwRunner() {
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

void registerCallbacks(GlfwRunner* runner) {
  glfwSetKeyCallback(runner->window, [](auto* window, int key, int scancode, int action, int mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return glfwSetWindowShouldClose(r->window, true);
    r->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(runner->window, [](auto* window, double xpos, double ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypox);
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
    r->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(runner->window, [](auto* window, int button, int action, int mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
    r->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(runner->window, [](auto* window, double xoffset, double yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
    r->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(runner->window, [](auto* window, int width, int height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
    r->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(runner->window, [](auto* window, int xpos, int ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    auto* r = (GlfwRunner*)glfwGetWindowUserPointer(window);
  });
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

bool GlfwRunner::shouldExit() {
  return window && glfwWindowShouldClose(window);
}

bool GlfwRunner::doInit() {
  if (!createGLFWWindow(this)) return false;
  registerCallbacks(this);
  ui.onInit(*this);
  return Runner::doInit();
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
  if (auto uiFlags = ui.updateAndRender(*this)) {
    if (isFlagSet(GLFW_RUNNER_UI_INPUT_HANDLED, uiFlags)) reset(input);
  }
  ui.onPostRender(*this);
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

}  // namespace uinta
