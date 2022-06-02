#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

struct GlfwRunner : Runner {
    GLFWwindow* window;

    GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {}
    ~GlfwRunner();

    virtual void doInit() override {}

    virtual void doPreTick(const RunnerState& state) override {}
    virtual void doTick(const RunnerState& state) override {}
    virtual void doPostTick(const RunnerState& state) override {}

    virtual void doPreRender() override {}
    virtual void doRender() override {}
    virtual void doPostRender() override {}

    virtual void doShutdown() override {}

protected:
    double getRuntime() override;
    void internalInit() override;
    void internalShutdown() override;
    void pollInput() override;
    bool shouldExit() override;
    void swap_buffers() override;
    void register_callbacks();

    void imguiInit();
    void imguiPreRender();
    void imguiPostRender();
    void imguiShutdown();

private:
    bool imguiEnabled = false;

};

void createGLFWWindow(GlfwRunner& view);

const char* getKeyStr(int key) noexcept;
const char* getActionStr(int action) noexcept;
const char* getModsStr(int mods) noexcept;

#endif // UINTA_GLFW_RUNNER_HPP
