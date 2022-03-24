#include <cstdio>
#include <exception>

#include <glad/glad.h>
#include <glfw.hpp>

void createGLFWWindow(viewport& view) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    printf("[INFO] Creating GLFW window \"%s\" (%dx%d)...\n", view.title, view.width, view.height);
    view.window = glfwCreateWindow(view.width, view.height, view.title, NULL, NULL);
    if (view.window == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }
    printf("[INFO] Completed creating GLFW window \"%s\" (%dx%d).\n", view.title, view.width, view.height);
    glfwMakeContextCurrent(view.window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    printf("[INFO] Loading GLAD...\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }
    printf("[INFO] Done loading GLAD.\n");
}
