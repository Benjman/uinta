#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <GLFW/glfw3.h>

#include <runner.hpp>

struct glfw_runner : runner {
    GLFWwindow* window;

    glfw_runner(const std::string& title, unsigned int width, unsigned int height) noexcept : runner(title, width, height) {}
    ~glfw_runner();

    virtual void doInit() override {}

    virtual void doPreTick(const runner_state& state) override {}
    virtual void doTick(const runner_state& state) override {}
    virtual void doPostTick(const runner_state& state) override {}

    virtual void doPreRender() override {}
    virtual void doRender() override {}
    virtual void doPostRender() override {}

    virtual void doShutdown() override {}

protected:
    double getRuntime() override;
    void internal_init() override;
    void internal_shutdown() override;
    void pollInput() override;
    bool shouldExit() override;
    void swap_buffers() override;
    void register_callbacks();

};

void createGLFWWindow(glfw_runner& view);

const char *getKeyStr(int key) noexcept;
const char *getActionStr(int action) noexcept;
const char *getModsStr(int mods) noexcept;

#endif // UINTA_GLFW_RUNNER_HPP
