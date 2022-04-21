#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./runner.hpp"
#include "../utils/debug_controller.hpp"

camera3dRunner runner;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

int main(const int argc, const char** argv) {
    debug_controller debug(WINDOW_WIDTH, WINDOW_HEIGHT);
    metrics_controller& metrics = debug.metrics;

    auto tick_m     = metrics.init_metric(METRIC_FLOAT, "tick");
    auto tick_t     = debug.create_timer("tick");
    auto tick_a     = running_avg(10);

    auto render_m   = metrics.init_metric(METRIC_FLOAT, "render");
    auto render_t   = debug.create_timer("render");
    auto render_a   = running_avg(20);

    auto text_m   = metrics.init_metric(METRIC_FLOAT, "text");
    auto text_t   = debug.create_timer("render");

    runner.init();
    debug.init();

    glfwSetKeyCallback(runner.view.window, key_callback);

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.view.window)) {
        dt = glfwGetTime() - time;
        time += dt;


        debug.reset_timer(tick_t);
        runner.tick(dt);
        tick_a.add(debug.duration_micro(tick_t));
        metrics.set(tick_m, tick_a.avg());

        debug.reset_timer(text_t);
        debug.mesh_metric(tick_m);
        debug.mesh_metric(render_m);
        debug.mesh_metric(text_m);
        metrics.set(text_m, (float) debug.duration_micro(text_t));


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        debug.reset_timer(render_t);
        debug.render();
        runner.render();
        render_a.add(debug.duration_micro(render_t));
        metrics.set(render_m, render_a.avg());

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
