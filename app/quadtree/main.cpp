#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./quadtreeRunner.hpp"

int main(const int argc, const char **argv) {
    quadtreeRunner runner;
    runner.init();

    while (!glfwWindowShouldClose(runner.view.window)) {
        runner.tick(glfwGetTime());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(runner.view.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

