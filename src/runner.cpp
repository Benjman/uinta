#include "spdlog/spdlog.h"
#include <glad/glad.h>

#include <logging.hpp>
#include <runner.hpp>

Display::Display(const std::string& title, unsigned int width, unsigned int height) noexcept :
    title(std::string(title)), width(width), height(height) {
    aspectRatio = (float) width / (float) height;
}

void Runner::init() {
    internal_init();
    doInit();
}

int Runner::run() {
    init();
    while (!shouldExit()) {
        pollInput();
        do {
            tick(getRuntime());
        } while(!shouldRenderFrame());
        render();
    }
    shutdown();
    return EXIT_SUCCESS;
}

void Runner::tick(float runtime) {
    state.delta = runtime - state.runtime;
    state.runtime = runtime;
    state.tick++;
    doPreTick(state);
    doTick(state);
    doPostTick(state);
    state.input.reset();
}

void Runner::render() {
    glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
    glClear(clear_mask);
    doPreRender();
    doRender();
    doPostRender();
    swap_buffers();
}

void Runner::shutdown() {
    internal_shutdown();
    doShutdown();
}

bool Runner::shouldRenderFrame() {
    // TODO Runner should have a `targetFps`, and this method will return true when a frame needs to be rendered based on `runtime - lastFrame >= targetFps`
    //      See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
    return true;
}

void Runner::setClearMask(const GLbitfield mask) {
    clear_mask = mask;
}

void Runner::setBackground(const glm::vec3& background) {
    background_color = background;
}

void Runner::handleCursorPositionChanged(const double xpos, const double ypos) {
    state.input.cursordx = xpos - state.input.cursorx;
    state.input.cursordy = ypos - state.input.cursory;
    state.input.cursorx = xpos;
    state.input.cursory = ypos;
}

void Runner::handleScrollInput(const double xoffset, const double yoffset) {
    spdlog::debug("Runner::handleScrollInput - Mouse scroll (%+.1f, %+.1f)", xoffset, yoffset);
    state.input.scrolldx = xoffset;
    state.input.scrolldy = yoffset;
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
    spdlog::debug("Runner::handleKeyInput - Key %s event: %s%s\n", getActionStr(action), getModsStr(mods), getKeyStr(key));
    if (action == ACTION_PRESS) state.input.keyPressed(key, mods);
    if (action == ACTION_RELEASE) state.input.keyReleased(key, mods);
    if (action == ACTION_REPEAT) state.input.keyRepeated(key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
    spdlog::debug("Runner::handleMouseButtonInput - Mouse %s event: %s%s\n", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    if (action == ACTION_PRESS) state.input.mouseButtonPressed(button, mods);
    if (action == ACTION_RELEASE) state.input.mouseButtonReleased(button, mods);
    state.input.flags = mods;
}

void Runner::handleWindowSizeChanged(const int width, const int height) {
    state.display.width = width;
    state.display.height = height;
}
