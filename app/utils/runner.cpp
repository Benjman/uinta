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

void runner::preRender() {
    doPreRender();
}

void runner::render() {
    doRender();
}

void runner::postRender() {
    doPostRender();
}
