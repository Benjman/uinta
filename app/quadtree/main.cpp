#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./quadtreeRunner.hpp"

int main(const int argc, const char **argv) {
    quadtreeRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.view.window)) {
        runner.tick(glfwGetTime());
        runner.render();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

