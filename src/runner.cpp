#include <glad/glad.h>

#include <runner.hpp>

viewport::viewport(const std::string& title, unsigned int width, unsigned int height) noexcept :
    title(std::string(title)), width(width), height(height) {
}

void runner::init() {
    internal_init();
    doInit();
}

void runner::tick(float runtime) {
    state.delta = runtime - state.runtime;
    state.runtime = runtime;
    state.tick++;
    doPreTick(state);
    doTick(state);
    doPostTick(state);
    state.input.reset();
}

void runner::render(const glm::vec3& clear_color, const GLbitfield clear_mask) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0);
    glClear(clear_mask);
    doPreRender();
    doRender();
    doPostRender();
    swap_buffers();
}

void runner::shutdown() {
    internal_shutdown();
    doShutdown();
}

bool runner::shouldRenderFrame() {
    // TODO Runner should have a `targetFps`, and this method will return true when a frame needs to be rendered based on `runtime - lastFrame >= targetFps`
    return true;
}

void runner::handleCursorPositionChanged(const double xpos, const double ypos) {
    state.input.cursordx = xpos - state.input.cursorx;
    state.input.cursordy = ypos - state.input.cursory;
    state.input.cursorx = xpos;
    state.input.cursory = ypos;
}

void runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
    printf("Key %s event: %s%s\n", getActionStr(action), getModsStr(mods), getKeyStr(key)); // TODO trace level
    if (action == ACTION_PRESS) state.input.keyPressed(key, mods);
    if (action == ACTION_RELEASE) state.input.keyReleased(key, mods);
    if (action == ACTION_REPEAT) state.input.keyRepeated(key, mods);
}
