#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./runner.hpp"
#include "../utils/debug_controller.hpp"

#include <math.hpp>

showcaseRunner runner;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

int main(const int argc, const char** argv) {
    debug_controller debug(WINDOW_WIDTH, WINDOW_HEIGHT);

    metric_t m_render       = debug.metrics.init_metric(METRIC_FLOAT, "render avg (10)");
    metric_t m_tick         = debug.metrics.init_metric(METRIC_UINT, "tick");

    debug_timer_t t_render = debug.create_timer("render");

    running_avg render_avg(10);

    runner.init();
    glfwSetKeyCallback(runner.view.window, key_callback);

    debug.init();

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.view.window)) {
        dt = glfwGetTime() - time;
        time += dt;

        runner.tick(dt);
        debug.metrics.set(m_tick, debug.metrics.getui(m_tick) + 1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        debug.mesh_metric(m_tick);
        debug.mesh_metric(m_render);

        debug.reset_timer(t_render);
        runner.render();
        debug.render();
        debug.metrics.set(m_render, (float) debug.duration_micro(t_render));

        render_avg.add((float) debug.duration_micro(t_render));
        debug.metrics.set(m_render, render_avg.avg());

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


