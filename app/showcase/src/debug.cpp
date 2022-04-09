#include<glad/glad.h>
#include <algorithm>
#include <cstdio>

#include "debug.hpp"

#include <font.hpp>
#include <shader.hpp>

void init_bufs(buffer_ctx&);
void upload_buf(const buffer_region&);
void init_shader(GLuint*);

void debug_controller::init(unsigned int view_width, unsigned int view_height) {
    std::fill(render_queue, render_queue + internal::DEBUG_CONTROLLER_MAX_TIMERS, -1);
    std::fill(render_queue_times, render_queue_times + internal::DEBUG_CONTROLLER_MAX_TIMERS, 0.0);
    font::load_font(font);
    init_shader(&shader);
    buf.vsize = sizeof(vbuf) / sizeof(GLfloat);
    buf.isize = sizeof(ibuf) / sizeof(GLuint);
    init_bufs(buf);
    upload_buf(render_time_buf);
}

debug_timer_t debug_controller::create_timer(const char* name) noexcept {
    for (int i = 0; i < internal::DEBUG_CONTROLLER_MAX_TIMERS; i++) {
        if (!timer_assignments[i]) {
            timer_assignments[i] = name;
            return i;
        }
    }
    printf("[WARN] Ignoring timer for '%s': No available debug timers.\n", name);
    return -1;
}

void debug_controller::reset_timer(const debug_timer_t handle) noexcept {
    if (handle == -1
        || handle > internal::DEBUG_CONTROLLER_MAX_TIMERS) {
        printf("[WARN] Ignoring reset timer attempt for invalid timer.\n");
        return;
    }
    timers[handle] = std::chrono::high_resolution_clock::now();
}

double debug_controller::duration(const debug_timer_t handle) noexcept {
    auto now = std::chrono::system_clock::now();
    if (handle == -1
        || handle > internal::DEBUG_CONTROLLER_MAX_TIMERS) {
        printf("[WARN] Ignoring stop timer attempt for invalid timer.\n");
        return -1.0;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - timers[handle]).count();
}

void debug_controller::render_timer(debug_timer_t handle, float time) {
    if (handle == -1
        || handle > internal::DEBUG_CONTROLLER_MAX_TIMERS) {
        printf("[WARN] Ignoring render timer attempt for invalid timer.\n");
        return;
    }
    for (int i = 0; i < internal::DEBUG_CONTROLLER_MAX_TIMERS; i++) {
        if (render_queue[i] == -1) {
            render_queue[i] = handle;
            render_queue_times[i] = time;
            return;
        }
    }
    printf("[WARN] render timer queue full for '%s'.\n", timer_assignments[handle]);
}

void debug_controller::render() {
    int queue_count = 0;
    buffer_region region = buffer_region();
    region.voffset = 0;
    region.ioffset = 0;
    unsigned int vcount = 0, icount = 0, ioff = 0;
    for (int i = 0; render_queue[i] != -1; i++, queue_count = i) {
        debug_timer_t handle = render_queue[i];
        const char* label = timer_assignments[handle];
        const int v = (int) std::min(render_queue_times[queue_count], 999.0f);
        font::text text = font::text(&font, std::string(label) + " " + std::to_string(v) + " ms", 18.0);
        text.color = vec3(1.0, 0.0, 0.0);
        text.pos = vec2(0.0, text.line_size * i);
        font::generate_mesh(&text, 1000, 1000, timer_mesh_attribs, &vbuf[vcount], &vcount, &ibuf[icount], &icount, &ioff);
    }
    if (!queue_count)
        return;
    region.vbuf = vbuf;
    region.ibuf = ibuf;
    region.vcount = vcount;
    region.voffset = 0;
    region.icount = icount;
    region.ioffset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, buf.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.ebo);
    upload_buf(region);

    std::fill(render_queue, render_queue + queue_count, -1);
    std::fill(render_queue_times, render_queue_times + queue_count, 0.0);

    glUseProgram(shader);
    glBindVertexArray(buf.vao);
    glDrawElements(GL_TRIANGLES, region.icount, GL_UNSIGNED_INT, 0);
}

void init_bufs(buffer_ctx& ctx) {
    glGenVertexArrays(1, &ctx.vao);
    GLuint ids[2];
    glGenBuffers(2, ids);
    ctx.vbo = ids[0];
    ctx.ebo = ids[1];

    glBindVertexArray(ctx.vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.ebo);

    glBufferData(GL_ARRAY_BUFFER, ctx.vsize, nullptr, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctx.isize, nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}

void upload_buf(const buffer_region& region) {
    glBufferSubData(GL_ARRAY_BUFFER, region.voffset, region.vcount * sizeof(GLfloat), region.vbuf);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, region.ioffset, region.icount * sizeof(GLuint), region.ibuf);
}

void init_shader(GLuint* shader) {
    const char *vshader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "layout (location = 1) in vec2 in_uv;"
        "layout (location = 2) in vec3 in_color;"
        "out vec2 pass_uv;"
        "out vec3 pass_color;"
        "void main() {"
        "  pass_uv = in_uv;"
        "  pass_color = in_color;"
        "  gl_Position = vec4(in_pos, 0.0, 1.0);"
        "}\0";

    const char *fshader =
        "#version 330 core\n"
        "in vec2 pass_uv;"
        "in vec3 pass_color;"
        "uniform sampler2D atlas;"
        "out vec4 out_color;"
        "void main() {"
        "  out_color = vec4(pass_color, texture(atlas, pass_uv).r);"
        "}\0";

    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    *shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum));
    glUseProgram(*shader);
}
