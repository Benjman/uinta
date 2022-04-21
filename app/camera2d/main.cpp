#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./camera2drunner.hpp"

#include <cstdio>

camera2dRunner runner;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

int main(const int argc, const char **argv) {
    runner.init();

    glfwSetKeyCallback(runner.view.window, key_callback);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept {
    printf("Key %s event: %s (%d)\n", getActionStr(action), getKeyStr(key), mods);
    runner.key_callback(key, scancode, action, mods);
}

