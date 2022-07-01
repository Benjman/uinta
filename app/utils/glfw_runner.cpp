#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/input.hpp>
#include <uinta/logging.hpp>

#include "glfw_runner.hpp"
#include "imgui_util.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace uinta;

GlfwRunner::~GlfwRunner() {
  if (window)
    glfwDestroyWindow(window);
  glfwTerminate();
}

bool GlfwRunner::internalInit() {
  createGLFWWindow(*this);
  if (window == NULL)
    return false;
  register_callbacks();
  imgui::init(window);
  return true;
}

void GlfwRunner::internalShutdown() { uinta::imgui::shutdown(window); }

void GlfwRunner::internalPreRender() { imgui::preRender(window); }

void GlfwRunner::internalPostRender() {
#ifndef IMGUI_API_DISABLED
  imgui::postRender(window);
#endif
}

void GlfwRunner::swapBuffers() { glfwSwapBuffers(window); }

void GlfwRunner::register_callbacks() {
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q)
      return glfwSetWindowShouldClose(runner->window, true);
    runner->handleKeyInput(key, scancode, action, mods);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
    GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
    runner->handleCursorPositionChanged(xpos, ypos);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
    GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
    runner->handleMouseButtonInput(button, action, mods);
  });

  glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
    GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
    runner->handleScrollInput(xoffset, yoffset);
  });

  glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
    runner->handleWindowSizeChanged(width, height);
  });

  // glfwSetWindowPosCallback(window, [](GLFWwindow *window, int xpos, int ypos) {
  //   GlfwRunner *runner = (GlfwRunner *)glfwGetWindowUserPointer(window);
  //   SPDLOG_LOGGER_INFO(runner->logger, "GLFW window pos {}, {}", xpos, ypos);
  // });
}

void uinta::createGLFWWindow(GlfwRunner &runner) {
  spdlog::stopwatch sw;
  logger_t logger = spdlog::stderr_color_mt("createGLFWWindow");
  SPDLOG_LOGGER_INFO(logger, "Initializing GLFW...", runner.display.title.c_str(), runner.display.width, runner.display.height);

  if (!glfwInit())
    return SPDLOG_LOGGER_ERROR(logger, "Failed to initialize GLFW!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  SPDLOG_LOGGER_INFO(logger, "Creating GLFW window \"{}\" ({}x{})...", runner.display.title.c_str(), runner.display.width,
                     runner.display.height);

  runner.window = glfwCreateWindow(runner.display.width, runner.display.height, runner.display.title.c_str(), NULL, NULL);
  if (runner.window == NULL)
    return SPDLOG_LOGGER_ERROR(logger, "Failed to create GLFW window!");

  glfwSetWindowUserPointer(runner.window, &runner);
  glfwMakeContextCurrent(runner.window);

  SPDLOG_LOGGER_INFO(logger, "Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return SPDLOG_LOGGER_ERROR(logger, "Failed to load GLAD!");

  SPDLOG_LOGGER_INFO(logger, "GLFW initialization completed in {} seconds", sw.elapsed().count());
}

double GlfwRunner::getRuntime() { return glfwGetTime(); }

void GlfwRunner::pollInput() { glfwPollEvents(); }

bool GlfwRunner::shouldExit() { return glfwWindowShouldClose(window); }
