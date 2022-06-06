#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/input.hpp>
#include <uinta/logging.hpp>

#include "glfw_runner.hpp"
#include "glfw_input.cpp"

#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "../../lib/imgui/imgui.cpp"
#include "../../lib/imgui/imgui_draw.cpp"
#include "../../lib/imgui/imgui_tables.cpp"
#include "../../lib/imgui/imgui_widgets.cpp"
#include "../../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../../lib/imgui/backends/imgui_impl_glfw.cpp"

GlfwRunner::~GlfwRunner() {
  if (window)
    glfwDestroyWindow(window);
  glfwTerminate();
}

void GlfwRunner::internalInit() {
  createGLFWWindow(*this);
  register_callbacks();
}

void GlfwRunner::internalShutdown() {}

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
}

void createGLFWWindow(GlfwRunner &runner) {
  logger_t logger = spdlog::stderr_color_mt("createGLFWWindow");
  SPDLOG_LOGGER_INFO(logger, "Initializing GLFW...", runner.display.title.c_str(), runner.display.width, runner.display.height);
  spdlog::stopwatch sw;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  SPDLOG_LOGGER_INFO(logger, "Creating GLFW window \"{}\" ({}x{})...", runner.display.title.c_str(), runner.display.width,
                     runner.display.height);
  runner.window = glfwCreateWindow(runner.display.width, runner.display.height, runner.display.title.c_str(), NULL, NULL);
  if (runner.window == NULL) {
    glfwTerminate();
    SPDLOG_LOGGER_ERROR(logger, "Failed to create GLFW window.");
    throw std::exception();
    return;
  }

  glfwSetWindowUserPointer(runner.window, &runner);
  glfwMakeContextCurrent(runner.window);

  SPDLOG_LOGGER_INFO(logger, "Loading GLAD...");
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SPDLOG_LOGGER_ERROR(logger, "Failed to load GLAD.");
    throw std::exception();
  }
  SPDLOG_LOGGER_INFO(logger, "GLFW initialization completed in {} seconds", sw.elapsed().count());
}

double GlfwRunner::getRuntime() { return glfwGetTime(); }

void GlfwRunner::pollInput() { glfwPollEvents(); }

bool GlfwRunner::shouldExit() { return glfwWindowShouldClose(window); }

void GlfwRunner::imguiInit() {
#ifdef IMGUI_API
  imguiEnabled = true;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
#endif // IMGUI_API
}

void GlfwRunner::imguiPreRender() {
#ifdef IMGUI_API
  if (!imguiEnabled)
    return;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
#endif // IMGUI_API
}
void GlfwRunner::imguiPostRender() {
#ifdef IMGUI_API
  if (!imguiEnabled)
    return;
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // IMGUI_API
}

void GlfwRunner::imguiShutdown() {
#ifdef IMGUI_API
  if (!imguiEnabled)
    return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  imguiEnabled = false;
#endif // IMGUI_API
}
