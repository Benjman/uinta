#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <exception>

#include <runner.h>
#include <window.hpp>

void runner::createWindow(unsigned int width, unsigned int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    view.glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (view.glfwWindow == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }
    glfwMakeContextCurrent(view.glfwWindow);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }

    window::width = width;
    window::height = height;
}
