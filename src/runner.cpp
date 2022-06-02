#include <glad/glad.h>

#include <logging.hpp>
#include <runner.hpp>

void setSpdlogLevel();

Display::Display(const std::string& title, unsigned int width, unsigned int height) noexcept :
    title(std::string(title)), width(width), height(height) {
    aspectRatio = (float) width / (float) height;
}

Runner::Runner(const std::string& title, unsigned int width, unsigned int height) noexcept : display(title, width, height) {
    state.display = Display(title, width, height);
    setSpdlogLevel();
    logger = spdlog::stdout_color_mt("Runner");
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
    SPDLOG_LOGGER_DEBUG(logger, "Runner::handleScrollInput - Mouse scroll (%+.1f, %+.1f)", xoffset, yoffset);
    state.input.scrolldx = xoffset;
    state.input.scrolldy = yoffset;
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
    SPDLOG_LOGGER_DEBUG(logger, "Runner::handleKeyInput - Key {} event: {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
    if (action == ACTION_PRESS) state.input.keyPressed(key, mods);
    if (action == ACTION_RELEASE) state.input.keyReleased(key, mods);
    if (action == ACTION_REPEAT) state.input.keyRepeated(key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
    SPDLOG_LOGGER_DEBUG(logger, "Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
    if (action == ACTION_PRESS) state.input.mouseButtonPressed(button, mods);
    if (action == ACTION_RELEASE) state.input.mouseButtonReleased(button, mods);
    state.input.flags = mods;
}

void Runner::handleWindowSizeChanged(const int width, const int height) {
    state.display.width = width;
    state.display.height = height;
}

void setSpdlogLevel() {

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_TRACE
    spdlog::set_level(spdlog::level::trace);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_INFO
    spdlog::set_level(spdlog::level::info);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_WARN
    spdlog::set_level(spdlog::level::warn);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_ERROR
    spdlog::set_level(spdlog::level::err);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_CRITICAL
    spdlog::set_level(spdlog::level::critical);
#endif

}
