#ifndef UINTA_APP_RUNNER_HPP
#define UINTA_APP_RUNNER_HPP

#include "input.hpp"

#include <glfw.hpp>

#include <glm/vec3.hpp>
#include <vector>

const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

struct runner_state final {
    input input_state;

    float dt;
    float runtime;

    unsigned int tick;

};

struct runner {
    viewport view;
    runner_state state;

    float cursorx = 0.0;
    float cursory = 0.0;

    runner(const std::string& title, unsigned int width, unsigned int height) noexcept : view(title, width, height) {}

    void init();
    void tick(float dt);
    void render(const glm::vec3& clear_color = DEFAULT_CLEAR_COLOR, const GLbitfield clear_mask = GL_COLOR_BUFFER_BIT);
    void shutdown();

    bool shouldRenderFrame();

    void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
    void handleCursorPositionChanged(const double xpos, const double ypos);

    virtual void doInit() {}

    virtual void doPreTick(const runner_state& state) {}
    virtual void doTick(const runner_state& state) {}
    virtual void doPostTick(const runner_state& state) {}

    virtual void doPreRender() {}
    virtual void doRender() {}
    virtual void doPostRender() {}

    virtual void doShutdown() {}

};

#endif // UINTA_APP_RUNNER_HPP