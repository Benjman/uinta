// clang-format off
#include <uinta/logging.hpp>
#include <spdlog/stopwatch.h>
// clang-format on

#include "./glfw_runner.hpp"

namespace uinta {
void registerCallbacks(GlfwRunner*);
bool createGLFWWindow(GlfwRunner*);

GlfwRunner::~GlfwRunner() {
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

bool GlfwRunner::doInit() {
  Runner::doInit();
  if (!createGLFWWindow(this)) return false;
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

bool GlfwRunner::shouldExit() {
  return glfwWindowShouldClose(window);
}

void GlfwRunner::doPreTick(RunnerState& state) {
  ui.onPreTick(*this);
  Runner::doPreTick(state);
}

void GlfwRunner::doTick(RunnerState& state) {
  ui.onTick(*this);
  Runner::doTick(state);
}

void GlfwRunner::doPostTick(RunnerState& state) {
  ui.onPostTick(*this);
  Runner::doPostTick(state);
}

void GlfwRunner::doPreRender(RunnerState& state) {
  ui.onPreRender(*this);
  Runner::doPreRender(state);
}

void GlfwRunner::doRender(RunnerState& state) {
  ui.onRender(*this);
  Runner::doRender(state);
  auto uiResult = ui.updateAndRender(*this);
  if (uiResult.flags) {
    if (isFlagSet(uiResult.flags, GlfwRunnerUiResult::SHOW_GRID_FLAG)) setGridEnabled(!isFlagSet(flags, RUNNER_FLAG_GRID));
    if (isFlagSet(uiResult.flags, GlfwRunnerUiResult::INPUT_HANDLED)) {
      // TODO how do we get the engine to ignore input from here?
    }
  }
}

void GlfwRunner::doPostRender(RunnerState& state) {
  ui.onPostRender(*this);
  Runner::doPostRender(state);
}

void GlfwRunner::doShutdown() {
  ui.onShutdown(*this);
}

void GlfwRunner::doHandleWindowSizeChanged(const int width, const int height) {
  Runner::doHandleWindowSizeChanged(width, height);
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
  glfwSetKeyCallback(runner->window, [](auto* window, int key, int scancode, int action, int mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return glfwSetWindowShouldClose(runner->window, true);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(runner->window, [](auto* window, double xpos, double ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypox);
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(runner->window, [](auto* window, int button, int action, int mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(runner->window, [](auto* window, double xoffset, double yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(runner->window, [](auto* window, int width, int height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(runner->window, [](auto* window, int xpos, int ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    auto* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
  });
}

}  // namespace uinta
