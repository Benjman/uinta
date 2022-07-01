#ifndef UINTA_IMGUI_UTILS_HPP
#define UINTA_IMGUI_UTILS_HPP

#include <GLFW/glfw3.h>

namespace uinta {
class Camera;

namespace imgui {

void init(GLFWwindow *const);
void preRender(GLFWwindow *const);
void render(GLFWwindow *const);
void postRender(GLFWwindow *const);
void shutdown(GLFWwindow *const);

void camera(const Camera &);

} // namespace imgui
} // namespace uinta

#endif // UINTA_IMGUI_UTILS_HPP
