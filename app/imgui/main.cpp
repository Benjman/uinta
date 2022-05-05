#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

#include <debug.hpp>
#include <glfw.hpp>

struct runner {
    viewport view;
    unsigned int frame = 0;

    ~runner() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void init() {
        view.width = 1000;
        view.height = 1000;
        view.title = "hello imgui";
        createGLFWWindow(view);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(view.window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void pre_render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void render() {
        ImGui::Begin("Hello ImGUI");
        ImGui::Text("Frame %i", ++frame);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void post_render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void tick(float dt) {}

};

GLFWwindow* window = nullptr;
void on_exit_handler(int status, void *arg) {
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

int main(const int argc, const char **argv) {
    runner runner;
    runner.init();
    window = runner.view.window;

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();

        dt = glfwGetTime() - time;
        time += dt;

        runner.tick(dt);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.pre_render();
        runner.render();
        runner.post_render();

        glfwSwapBuffers(window);
    }

    on_exit(on_exit_handler, nullptr);

    return 0;
}

