#include <glad/glad.h>
#include "./runners/quadtreeRunner.hpp"

int main(const int argc, const char **argv) {
    quadtreeRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.view.glfwWindow)) {
        runner.update(glfwGetTime());
        runner.render();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
