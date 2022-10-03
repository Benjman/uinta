#ifndef UINTA_IMGUI_UTILS_HPP
#define UINTA_IMGUI_UTILS_HPP

class GLFWwindow;

namespace uinta {
class Camera;

namespace imgui {  // uinta::imgui
void init(GLFWwindow* const);
void preRender(GLFWwindow* const);
void render(GLFWwindow* const);
void postRender(GLFWwindow* const);
void shutdown(GLFWwindow* const);

namespace view {  // uinta::imgui::view
void camera(const Camera&);

}  // namespace view
}  // namespace imgui
}  // namespace uinta

#endif  // UINTA_IMGUI_UTILS_HPP
