#include <glad/glad.h>

#include "debug_controller.hpp"

#include <uinta/file.hpp>
#include <uinta/logging.hpp>
#include <uinta/shader.hpp>

#include <chrono>
#include <cmath>
#include <cstring>

using namespace font;
const std::unordered_map<FontMeshAttribTypes, FontMeshAttrib> attribs = {
    {FontMeshAttrib_Position, FontMeshAttrib(7, 0)},
    {FontMeshAttrib_UV,       FontMeshAttrib(7, 2)},
    {FontMeshAttrib_Color,    FontMeshAttrib(7, 4)},
};

DebugController::DebugController(unsigned int view_width, unsigned int view_height) noexcept
    : view_size(view_width, view_height) {}

void DebugController::init() {
  vbo.max = sizeof(vbuf) / sizeof(GLfloat);
  ebo.max = sizeof(ibuf) / sizeof(GLuint);
  initShader();
  initBuffers();
  initFont();
}

void DebugController::initBuffers() {
  glGenVertexArrays(1, &vao);
  GLuint ids[2];
  glGenBuffers(2, ids);
  vbo.id = ids[0];
  ebo.id = ids[1];

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

  glBufferData(GL_ARRAY_BUFFER, vbo.max, nullptr, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.max, nullptr, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  uploadBuffers();
}

void DebugController::initFont() {
  auto type   = font::ProggyCleanTT_Nerd_Font_Complete_Mono;
  font_handle = font::initFont(type, 256, 256);
  unsigned char data[getFileSize(getFontPath(type))];
  readFileBinary(getFontPath(type), (char *)data);
  font::load_font(font_handle, data);
}

void DebugController::initShader() {
  const char *vshader = "#version 330 core\n"
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

  const char *fshader = "#version 330 core\n"
                        "in vec2 pass_uv;"
                        "in vec3 pass_color;"
                        "uniform sampler2D atlas;"
                        "out vec4 out_color;"
                        "void main() {"
                        "  out_color = vec4(pass_color, texture(atlas, pass_uv).r);"
                        "}\0";

  const std::vector<std::string> sources({std::string(vshader, strlen(vshader)), std::string(fshader, strlen(fshader))});
  const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
  shader = createShaderProgram(sources, stages);

  glUseProgram(shader);
}

void DebugController::render() {
  if (!ebo.count)
    return;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

  uploadBuffers();

  glUseProgram(shader);
  glBindVertexArray(vao);

  glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);

  vbo.count = 0;
  ebo.count = 0;
  ioff      = 0;

  metric_row = 0;
}

void DebugController::uploadBuffers() {
  glBufferSubData(GL_ARRAY_BUFFER, vbo.offset, vbo.count * sizeof(GLfloat), vbuf);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, ebo.offset, ebo.count * sizeof(GLuint), ibuf);
}

debug_timer_t DebugController::createTimer() noexcept {
  for (int i = 0; i < DEBUG_CONTROLLER_MAX_TIMERS; i++) {
    if (!timerAssignments[i]) {
      timerAssignments[i] = true;
      resetTimer(i);
      return i;
    }
  }
  SPDLOG_WARN("No available debug timer storage.");
  return -1;
}

double DebugController::durationMicro(const debug_timer_t handle) noexcept {
  auto now = std::chrono::high_resolution_clock::now();
  if (handle == -1 || handle > DEBUG_CONTROLLER_MAX_TIMERS) {
    SPDLOG_WARN("Ignoring stop timer attempt for invalid timer.");
    return -1.0;
  }
  return std::chrono::duration_cast<std::chrono::microseconds>(now - timers[handle]).count();
}

double DebugController::durationMilli(const debug_timer_t handle) noexcept {
  auto now = std::chrono::high_resolution_clock::now();
  if (handle == -1 || handle > DEBUG_CONTROLLER_MAX_TIMERS) {
    SPDLOG_WARN("Ignoring stop timer attempt for invalid timer.");
    return -1.0;
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(now - timers[handle]).count();
}

void DebugController::meshMetric(const metric_t handle, const std::string append) {
  auto text    = font::text(metrics.assignments[handle], 20.0);
  text.color_r = 0.6;
  text.color_g = 0.6;
  text.color_b = 0.3;
  text.pos_y   = text.line_size * metric_row;
  switch (metrics.metric_type[handle]) {
  case METRIC_FLOAT:
    text.value += " " + std::to_string(metrics.getf(handle)) + " " + append;
    break;

  case METRIC_INT:
    text.value += " " + std::to_string(metrics.geti(handle)) + " " + append;
    break;

  case METRIC_UINT:
    text.value += " " + std::to_string(metrics.getui(handle)) + " " + append;
    break;
  }
  font::generate_mesh(&text, font_handle, view_size.x, view_size.y, &attribs, &vbuf[vbo.count], &vbo.count, &ibuf[ebo.count],
                      &ebo.count, &ioff);
  metric_row++;
}

void DebugController::resetTimer(const debug_timer_t handle) noexcept {
  if (handle == -1 || handle > DEBUG_CONTROLLER_MAX_TIMERS) {
    SPDLOG_WARN("Ignoring reset timer attempt for invalid timer.");
    return;
  }
  timers[handle] = std::chrono::high_resolution_clock::now();
}
