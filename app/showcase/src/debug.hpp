#ifndef SHOWCASE_DEBUG_CONTROLLER_HPP
#define SHOWCASE_DEBUG_CONTROLLER_HPP

#include <GL/gl.h>
#include <chrono>
#include <font.hpp>

using debug_timer_t = int;

constexpr unsigned int DEBUG_CONTROLLER_MAX_TIMERS = 8;

struct buffer_region {
    GLuint vbo = 0;
    GLuint ebo = 0;

    float* vbuf = 0;
    unsigned int vmax = 0;
    unsigned int voffset = 0;

    GLuint* ibuf = 0;
    unsigned int imax = 0;
    unsigned int ioffset = 0;

    unsigned int vcount = 0;
    unsigned int icount = 0;

};

struct buffer_ctx {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei vsize;
    GLsizei isize;
};

struct debug_controller {
    GLuint shader;

    font::font_ctx font = font::font_ctx(font::ProggyCleanTT_Nerd_Font_Complete_Mono, 256, 256);

    buffer_ctx buf;
    buffer_region render_time_buf;

    // vertex buffer
    float vbuf[15 * 1024];

    // index buffer
    unsigned int ibuf[15 * 1024];
    unsigned int icount = 0;


    void init(unsigned int view_width, unsigned int view_height);
    void init_font();

    debug_timer_t create_timer(const char* name) noexcept;
    void reset_timer(const debug_timer_t handle) noexcept;
    double duration(const debug_timer_t handle) noexcept;

    void render();
    void render_timer(debug_timer_t handle, float time);

private:
    std::chrono::time_point<std::chrono::system_clock> timers[DEBUG_CONTROLLER_MAX_TIMERS]{};
    const char* timer_assignments[DEBUG_CONTROLLER_MAX_TIMERS]{nullptr};
    debug_timer_t render_queue[DEBUG_CONTROLLER_MAX_TIMERS];
    float render_queue_times[DEBUG_CONTROLLER_MAX_TIMERS];
    std::string render_formats[DEBUG_CONTROLLER_MAX_TIMERS];

};

#endif // !SHOWCASE_DEBUG_CONTROLLER_HPP
