#include <glad/glad.h>

#include "./camera_2drunner.hpp"

#include <cstdio>

int main(const int argc, const char **argv) {
    camera2DRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.view.glfwWindow)) {
        runner.tick(glfwGetTime());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(runner.view.glfwWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
