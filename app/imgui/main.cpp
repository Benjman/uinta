#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define UINTA_APP_UTILS_IMPL
#include "../utils/utils.hpp"

#include <cstdlib>

struct ImGUIRunner final : GlfwRunner {
  unsigned int frame = 0;

  ImGUIRunner() : GlfwRunner("hello imgui", 1000, 1000) {}

  void doInit() override { imguiInit(); }

  void doPreRender() override { imguiPreRender(); }

  void doRender() override {
    ImGui::Begin("Hello ImGUI");
    ImGui::Text("Frame %i", ++frame);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  void doPostRender() override { imguiPostRender(); }

  void doShutdown() override { imguiShutdown(); }
};

ImGUIRunner runner;

int main(const int argc, const char **argv) { return runner.run(); }
