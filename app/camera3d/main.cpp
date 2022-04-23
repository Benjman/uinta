#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./runner.hpp"
#include "../utils/debug_controller.hpp"

camera3dRunner runner;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

int main(const int argc, const char** argv) {
    debug_controller debug(WINDOW_WIDTH, WINDOW_HEIGHT);
    metrics_controller& metrics = debug.metrics;

    auto timer = debug.create_timer();

    // register metrics
    auto tick_m     = metrics.init_metric(METRIC_FLOAT, "tick");
    auto text_m   = metrics.init_metric(METRIC_FLOAT, "text");
    auto render_m   = metrics.init_metric(METRIC_FLOAT, "render");

    // register averages
    auto render_a   = running_avg(20);

    runner.init();
    debug.init();

    glfwSetKeyCallback(runner.view.window, key_callback);

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.view.window)) {
        dt = glfwGetTime() - time;
        time += dt;

        debug.reset_timer(timer);
        runner.tick(dt);
        metrics.set(tick_m, (float) debug.duration_micro(timer));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        debug.reset_timer(timer);
        debug.render();
        runner.render();
        render_a.add(debug.duration_micro(timer));
        metrics.set(render_m, render_a.avg());

        debug.mesh_metric(tick_m);
        debug.mesh_metric(render_m);
        debug.mesh_metric(text_m);
        metrics.set(text_m, (float) debug.duration_micro(timer));

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
