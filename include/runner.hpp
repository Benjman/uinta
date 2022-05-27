#ifndef UINTA_APP_RUNNER_HPP
#define UINTA_APP_RUNNER_HPP

#include <input.hpp>

#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

struct viewport {
    std::string title;
    unsigned int width;
    unsigned int height;

    viewport(const std::string& title, const unsigned int width, const unsigned int height) noexcept;
};

struct runner_state final {
    /// number of times the tick cycle has been executed
    unsigned int tick;

    /// time in seconds since last tick cycle
    float delta;

    /// time in secinds the runner has been ticking
    float runtime;

    input_state input;
};

struct runner {
    viewport view;
    runner_state state;

    runner(const std::string& title, unsigned int width, unsigned int height) noexcept : view(title, width, height) {}
 
    void init();
    void tick(float dt);
    void render(const glm::vec3& clear_color = DEFAULT_CLEAR_COLOR, const GLbitfield clear_mask = GL_COLOR_BUFFER_BIT);
    void shutdown();

    bool shouldRenderFrame();

    void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
    void handleMouseButtonInput(const int button, const int action, const int mods);
    void handleCursorPositionChanged(const double xpos, const double ypos);

    virtual void doInit() {}

    virtual void doPreTick(const runner_state& state) {}
    virtual void doTick(const runner_state& state) {}
    virtual void doPostTick(const runner_state& state) {}

    virtual void doPreRender() {}
    virtual void doRender() {}
    virtual void doPostRender() {}

    virtual void doShutdown() {}

protected:
    virtual void internal_init() = 0;
    virtual void internal_shutdown() = 0;
    virtual void swap_buffers() = 0;

};

#endif // UINTA_APP_RUNNER_HPP
