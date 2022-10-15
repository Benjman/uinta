#include "./glfw_runner.hpp"

#include <spdlog/stopwatch.h>

#include "./imgui_util.hpp"

using namespace uinta;

GlfwRunner::~GlfwRunner() {
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

bool GlfwRunner::internalInit() {
  createGLFWWindow(*this);
  if (window == NULL) return false;
  register_callbacks();
  imgui::init(window);
  return true;
}

void GlfwRunner::register_callbacks() {
  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    SPDLOG_TRACE("Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return glfwSetWindowShouldClose(runner->window, true);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    SPDLOG_TRACE("Mouse position event x:{} y:{}", xpos, ypox);
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    SPDLOG_TRACE("Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
    SPDLOG_TRACE("Mouse scroll event x:{} y:{}", xoffset, yoffset);
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    SPDLOG_DEBUG("Window size updated: {}x{}.", width, height);
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
    runner->handleWindowSizeChanged(width, height);
  });

  glfwSetWindowPosCallback(window, [](GLFWwindow* window, int xpos, int ypos) {
    SPDLOG_DEBUG("Window position updated: {}x{}.", xpos, ypos);
    GlfwRunner* runner = (GlfwRunner*)glfwGetWindowUserPointer(window);
  });
}

void GlfwRunner::pollInput() {
  glfwPollEvents();
}

double GlfwRunner::getRuntime() {
  return glfwGetTime();
}

void GlfwRunner::swapBuffers() {
  glfwSwapBuffers(window);
}

void GlfwRunner::internalPreRender() {
  imgui::preRender(window);
}

void GlfwRunner::internalRender() {
  imgui::render(window);
}

void GlfwRunner::internalPostRender() {
  glDisable(GL_DEPTH_TEST);
  imgui::view::camera(camera);
  imgui::postRender(window);
}

void GlfwRunner::internalShutdown() {
  uinta::imgui::shutdown(window);
}

bool GlfwRunner::shouldExit() {
  return glfwWindowShouldClose(window);
}

void uinta::createGLFWWindow(GlfwRunner& runner) {
  spdlog::stopwatch sw;

  int version_major = 3;
  int version_minor = 3;

  SPDLOG_INFO("Initializing GLFW v{}.{} with OpenGL Core profile...", version_major, version_minor);

  if (!glfwInit()) return SPDLOG_ERROR("Failed to initialize GLFW!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  runner.window = glfwCreateWindow(runner.display.width, runner.display.height, runner.display.title.c_str(), NULL, NULL);
  if (!runner.window) return SPDLOG_ERROR("Failed to create GLFW window!");
  SPDLOG_INFO("Created window {}x{} (aspect ratio {}).", runner.display.width, runner.display.height, runner.display.aspectRatio);

  glfwSetWindowUserPointer(runner.window, &runner);
  glfwMakeContextCurrent(runner.window);

  SPDLOG_INFO("Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return SPDLOG_ERROR("Failed to load GLAD!");

  SPDLOG_INFO("GLFW initialization completed in {} seconds.", sw.elapsed().count());
}
