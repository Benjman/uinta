#include "./runner.hpp"

#include <glfw.hpp>

void runner::init() {
    createGLFWWindow(&view);
    doInit();
}

void runner::tick(float runtime) {
    state.dt = runtime - state.runtime;
    state.runtime = runtime;
    state.tick++;
    doPreTick(state);
    doTick(state);
    doPostTick(state);
    state.input_state.reset();
}

void runner::render(const glm::vec3& clear_color, const GLbitfield clear_mask) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0);
    glClear(clear_mask);
    doPreRender();
    doRender();
    doPostRender();
    glfwSwapBuffers(view.window);
}

void runner::shutdown() {
    doShutdown();
}

bool runner::shouldRenderFrame() {
    // TODO Runner should have a `targetFps`, and this method will return true when a frame needs to be rendered based on `runtime - lastFrame >= targetFps`
    return true;
}

void runner::handleCursorPositionChanged(const double xpos, const double ypos) {
    state.input_state.cursordx = xpos - state.input_state.cursorx;
    state.input_state.cursordy = ypos - state.input_state.cursory;
    state.input_state.cursorx = xpos;
    state.input_state.cursory = ypos;
}

void runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
    printf("Key %s event: %s%s\n", getActionStr(action), getModsStr(mods), getKeyStr(key));
    if (action == GLFW_PRESS) state.input_state.keyPressed(key, mods);
    if (action == GLFW_RELEASE) state.input_state.keyReleased(key, mods);
    if (action == GLFW_REPEAT) state.input_state.keyRepeated(key, mods);
}
