#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

#include <debug.hpp>

struct imgui_runner final : glfw_runner {
    unsigned int frame = 0;
    
    imgui_runner() : glfw_runner("hello imgui", 1000, 1000) {}

    void doInit() override {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void doPreRender() override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void doRender() override {
        ImGui::Begin("Hello ImGUI");
        ImGui::Text("Frame %i", ++frame);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void doPostRender() override {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void doShutdown() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

};

imgui_runner runner;

int main(const int argc, const char **argv) {
    runner.init();

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.window)) {
        glfwPollEvents();

        dt = glfwGetTime() - time;
        time += dt;
        while (!runner.shouldRenderFrame())
            runner.tick(glfwGetTime());
        runner.render();
    }

    runner.shutdown();
    on_exit([] (int status, void* arg) {
        if (runner.window)
            glfwDestroyWindow(runner.window);
        glfwTerminate();
    }, nullptr);
    return 0;
}
