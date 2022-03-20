#include <glad/glad.h>
#include "./runners/quadtreeRunner.hpp"

int main(const int argc, const char **argv) {
    quadtreeRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.view.glfwWindow)) {
        runner.tick(glfwGetTime());
        runner.render();
    }

    glfwTerminate();
    return 0;
}