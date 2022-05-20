#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

#include <debug.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

struct imgui_runner final : runner {
    unsigned int frame = 0;
    
    imgui_runner() : runner("hello imgui", 1000, 1000) {}

    ~imgui_runner() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void doInit() override {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(view.window, true);
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

};

imgui_runner runner;

int main(const int argc, const char **argv) {
    runner.init();

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();

        dt = glfwGetTime() - time;
        time += dt;

        runner.preTick(dt);
        runner.tick(dt);
        runner.postTick(dt);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.preRender();
        runner.render();
        runner.postRender();
    }

    runner.shutdown();
    on_exit([] (int status, void* arg) {
        if (runner.view.window)
            glfwDestroyWindow(runner.view.window);
        glfwTerminate();
    }, nullptr);
    return 0;
}
