#ifndef UINTA_APP_RUNNER_HPP
#define UINTA_APP_RUNNER_HPP

#include <glfw.hpp>

#include <string>

struct runner {
    viewport view;

    runner(const std::string& title, unsigned int width, unsigned int height) noexcept : view(title, width, height) {}

    void init();
    void preTick(float dt);
    void tick(float dt);
    void postTick(float dt);
    void preRender();
    void render();
    void postRender();

    virtual void doInit() {}
    virtual void doPreTick(float dt) {}
    virtual void doTick(float dt) {}
    virtual void doPostTick(float dt) {}
    virtual void doPreRender() {}
    virtual void doRender() {}
    virtual void doPostRender() {}

    virtual void doKeyCallback(int key, int scancode, int action, int mods) {}

};

#endif // UINTA_APP_RUNNER_HPP
