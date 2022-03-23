#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./camera_2drunner.hpp"

#include <cstdio>

int main(const int argc, const char **argv) {
    camera2DRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.glfw.window)) {
        runner.tick(glfwGetTime());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(runner.glfw.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
