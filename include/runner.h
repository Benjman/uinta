#ifndef UINTA_RUNNER_H
#define UINTA_RUNNER_H

#include <GLFW/glfw3.h>

struct viewport {
    unsigned int width;
    unsigned int height;
    char *title;

    GLFWwindow *glfwWindow;
};

struct runner {
    viewport view;

    virtual void init() {}
    virtual void render() {}
    virtual void update(float dt) {}

    void createWindow(unsigned int width, unsigned int height, const char *title);

};

#endif // UINTA_RUNNER_H
