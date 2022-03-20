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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    view.width = width;
    view.height = height;
    view.title = title;
    
    printf("[INFO] Creating GLFW window...\n");
    printf("[DEBUG] GLFW window size %ix%i\n", view.width, view.height);
    printf("[DEBUG] GLFW window title \"%s\"\n", view.title.c_str());
    view.glfwWindow = glfwCreateWindow(view.width, view.height, view.title.c_str(), NULL, NULL);
    if (view.glfwWindow == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }
    printf("[INFO] Completed creating GLFW window.\n");
    glfwMakeContextCurrent(view.glfwWindow);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    printf("[INFO] Loading GLAD...\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }
    printf("[INFO] Done loading GLAD.\n");
}
