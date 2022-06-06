#ifndef SHOWCASE_DEBUG_CONTROLLER_HPP
#define SHOWCASE_DEBUG_CONTROLLER_HPP

#include <uinta/buffer.hpp>
#include <uinta/font.hpp>
#include <uinta/macros.hpp>
#include <uinta/math.hpp>
#include <uinta/metrics.hpp>

#include <chrono>

#include <glm/vec2.hpp>

namespace uinta {

// TODO move all timer stuff to its own controller
using debug_timer_t = int;

constexpr unsigned int DEBUG_CONTROLLER_MAX_TIMERS = 8;

struct DebugController final {
  GLuint shader;

  GLuint vao;
  gl_buf vbo;
  gl_buf ebo;

  GLfloat vbuf[KILOBYTES(15)];
  GLuint ibuf[KILOBYTES(15)];

  glm::vec2 view_size;

  font::font_t font_handle;

  MetricsController metrics;
  unsigned int metric_row = 0, ioff = 0;

  DebugController(unsigned int view_width, unsigned int view_height) noexcept;

  void init();
  void initBuffers();
  void initFont();
  void initShader();
  void render();
  void uploadBuffers();

  debug_timer_t createTimer() noexcept;
  double durationMicro(const debug_timer_t handle) noexcept;
  double durationMilli(const debug_timer_t handle) noexcept;
  void meshMetric(const metric_t handle, const std::string append = "");
  void resetTimer(const debug_timer_t handle) noexcept;

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> timers[DEBUG_CONTROLLER_MAX_TIMERS]{};
  bool timerAssignments[DEBUG_CONTROLLER_MAX_TIMERS]{false};
};

} // namespace uinta

#endif // !SHOWCASE_DEBUG_CONTROLLER_HPP
