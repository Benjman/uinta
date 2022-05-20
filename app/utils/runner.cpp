#include "./runner.hpp"

#include <glfw.hpp>

void runner::init() {
    createGLFWWindow(&view);
    doInit();
}

void runner::preTick(float dt) {
    doPreTick(dt);
}

void runner::tick(float dt) {
    doTick(dt);
}

void runner::postTick(float dt) {
    doPostTick(dt);
}

void runner::preRender(const glm::vec3& clear_color, const GLbitfield clear_mask) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0);
    glClear(clear_mask);
    doPreRender();
}

void runner::render() {
    doRender();
}

void runner::postRender() {
    doPostRender();
    glfwSwapBuffers(view.window);
}

void runner::shutdown() {
    doShutdown();
}
