#ifndef SHOWCASE_DEBUG_CONTROLLER_HPP
#define SHOWCASE_DEBUG_CONTROLLER_HPP

#include <chrono>

#include <buffer.hpp>
#include <font.hpp>
#include <macros.hpp>
#include <math.hpp>
#include <metrics.hpp>

// TODO move all timer stuff to its own controller
using debug_timer_t = int;

constexpr unsigned int DEBUG_CONTROLLER_MAX_TIMERS = 8;

struct debug_controller final {
    GLuint shader;

    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    GLfloat vbuf[KILOBYTES(15)];
    GLuint ibuf[KILOBYTES(15)];

    vec2 view_size;

    font::font_t font_handle;

    metrics_controller metrics;
    unsigned int metric_row = 0, ioff = 0;

    debug_controller(unsigned int view_width, unsigned int view_height) noexcept;

    void init();
    void init_buffers();
    void init_font();
    void init_shader();
    void render();
    void upload_buffers();

    debug_timer_t create_timer() noexcept;
    double duration_micro(const debug_timer_t handle) noexcept;
    double duration_milli(const debug_timer_t handle) noexcept;
    void mesh_metric(const metric_t handle, const std::string append = "");
    void reset_timer(const debug_timer_t handle) noexcept;

private:
    std::chrono::time_point<std::chrono::system_clock> timers[DEBUG_CONTROLLER_MAX_TIMERS]{};
    bool timer_assignments[DEBUG_CONTROLLER_MAX_TIMERS]{false};

};

#endif // !SHOWCASE_DEBUG_CONTROLLER_HPP
