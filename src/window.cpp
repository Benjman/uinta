#include <cstdio>
#include <exception>

#include <glad/glad.h>
#include <window.hpp>

void createGLFWWindow(glfwdto& dto) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    printf("[INFO] Creating GLFW window \"%s\" (%dx%d)...\n", dto.title, dto.width, dto.height);
    dto.window = glfwCreateWindow(dto.width, dto.height, dto.title, NULL, NULL);
    if (dto.window == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }
    printf("[INFO] Completed creating GLFW window \"%s\" (%dx%d).\n", dto.title, dto.width, dto.height);
    glfwMakeContextCurrent(dto.window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    printf("[INFO] Loading GLAD...\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }
    printf("[INFO] Done loading GLAD.\n");
}
