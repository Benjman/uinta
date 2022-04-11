#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./runner.hpp"
#include "./src/debug.hpp"
#include "./src/text_defs.hpp"

#include <cstdio>

showcaseRunner runner;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

unsigned int tick = 0;

int main(const int argc, const char **argv) {
    debug_controller debug;
    double tick_time, render_time, init_time;
    running_avg render_avg = running_avg(10);

    buffer_region item;
    item.vbuf = &debug.vbuf[0];
    item.ibuf = &debug.ibuf[0];
    item.voffset = 0;
    item.ioffset = 0;

    int tick_timer_handle = debug.create_timer("TICK TIME");
    int render_timer_handle = debug.create_timer("RENDER TIME");
    int frame_handle = debug.create_timer("FRAME");
    int time_handle = debug.create_timer("TIME");

    debug.reset_timer(tick_timer_handle);
    runner.init();
    debug.init(runner.view.width, runner.view.height);
    init_time = debug.duration(tick_timer_handle);

    glfwSetKeyCallback(runner.view.window, key_callback);

    while (!glfwWindowShouldClose(runner.view.window)) {
        debug.reset_timer(tick_timer_handle);
        runner.tick(glfwGetTime());
        tick_time = debug.duration(tick_timer_handle);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        debug.reset_timer(render_timer_handle);
        runner.render();
        render_time = debug.duration(tick_timer_handle);

        debug.render_timer(time_handle, glfwGetTime());
        debug.render_timer(frame_handle, ++tick);
        debug.render_timer(tick_timer_handle, tick_time);
        debug.render_timer(render_timer_handle, render_time);
        debug.render();

        glfwSwapBuffers(runner.view.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept {
    printf("Key %s event: %s (%d)\n", getActionStr(action), getKeyStr(key), mods);
    runner.key_callback(key, scancode, action, mods);
}


