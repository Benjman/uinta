#ifndef UINTA_WINDOW_H
#define UINTA_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

struct viewport {
    GLFWwindow* window;
    std::string title;
    unsigned int width;
    unsigned int height;

    viewport(const std::string& title, const unsigned int width, const unsigned int height) noexcept;
};

void createGLFWWindow(viewport* const view);

const char *getKeyStr(int key) noexcept;
const char *getActionStr(int action) noexcept;
const char *getModsStr(int mods) noexcept;

#endif // UINTA_WINDOW_H
