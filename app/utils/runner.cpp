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

void runner::preRender(const glm::vec3& clear_color) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    doPreRender();
}

void runner::render() {
    doRender();
}

void runner::postRender() {
    doPostRender();
}
