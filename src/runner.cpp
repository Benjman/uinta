// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_flags.hpp>

namespace uinta {

void clearBuffer(const glm::vec3& color, GLbitfield mask);

const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

void setSpdlogLevel();

Runner::Runner(const std::string& title, uint32_t width, uint32_t height) noexcept : display(title, width, height) {
  SPDLOG_INFO("Runner started for '{}'.", title);
  state.display = Display(title, width, height);
  setSpdlogLevel();
  setBackground(DEFAULT_CLEAR_COLOR);
  setGridEnabled(true);
  setFlag(RUNNER_FLAG_CAMERA, true, flags);

  if (isCameraEnabled(flags)) {
    force(camera.angle, 45);
    force(camera.pitch, 45);
    force(camera.dist, 5);
  }
}

bool Runner::init() {
  spdlog::stopwatch sw{};
  fileManager.init();
  fileManager.loadAll();
  if (!doInit()) return false;
  grid.init(fileManager);
  SPDLOG_INFO("Completed initialization for '{}' in {} seconds.", display.title, sw.elapsed().count());
  return true;
}

int Runner::run() {
  try {
    if (!init()) {
      SPDLOG_ERROR("Failed to initialize runner! Exiting application.");
      return EXIT_FAILURE;
    }
    auto lastTick = getRuntime();
    while (!shouldExit()) {
      do {
        tick(state.delta = getRuntime() - lastTick);
        lastTick += state.delta;
      } while (!shouldRenderFrame(state.delta));
      reset(state.input);
      pollInput();
      render();
    }
    shutdown();
    return EXIT_SUCCESS;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Window '{}' encountered an error: {}", display.title, ex.what());
    throw ex;
  }
}

void Runner::tick(float dt) {
  state.delta = dt;
  state.runtime += dt;
  state.tick++;

  doPreTick(state);
  doTick(state);
  doPostTick(state);
}

void Runner::render() {
  doPreRender(state);
  doRender(state);
  doPostRender(state);
}

inline void clearBuffer(const glm::vec3& color, GLbitfield mask) {
  glClearColor(color.r, color.g, color.b, 1.0);
  glClear(mask);
}

void Runner::shutdown() {
  SPDLOG_INFO("Shutdown requested for '{}'.", display.title);
  doShutdown();
}

bool Runner::shouldRenderFrame(float dt) {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::setClearMask(const GLbitfield mask) {
  clearMask = mask;
}

void Runner::setBackground(const glm::vec3& color) {
  background_color = color;
}

void Runner::handleCursorPositionChanged(const double xpos, const double ypos) {
  state.input.cursordx = xpos - state.input.cursorx;
  state.input.cursordy = ypos - state.input.cursory;
  state.input.cursorx = xpos;
  state.input.cursory = ypos;
}

void Runner::handleScrollInput(const double xoffset, const double yoffset) {
  state.input.scrolldx = xoffset;
  state.input.scrolldy = yoffset;
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
  if (action == ACTION_PRESS) keyPressed(state.input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(state.input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(state.input, key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
  if (action == ACTION_PRESS) mouseButtonPressed(state.input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(state.input, button, mods);
  state.input.flags = mods;
}

void Runner::handleWindowSizeChanged(const int width, const int height) {
  state.display.width = width;
  state.display.height = height;
  doHandleWindowSizeChanged(width, height);
}

void setSpdlogLevel() {
#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_TRACE
  spdlog::set_level(spdlog::level::trace);
  SPDLOG_INFO("Logging level set to trace.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_DEBUG
  spdlog::set_level(spdlog::level::debug);
  SPDLOG_INFO("Logging level set to debug.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_INFO
  spdlog::set_level(spdlog::level::info);
  SPDLOG_INFO("Logging level set to info.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_WARN
  spdlog::set_level(spdlog::level::warn);
  SPDLOG_INFO("Logging level set to warn.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_ERROR
  spdlog::set_level(spdlog::level::err);
  SPDLOG_INFO("Logging level set to error.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_CRITICAL
  spdlog::set_level(spdlog::level::critical);
  SPDLOG_INFO("Logging level set to critical.");
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_OFF
  spdlog::set_level(spdlog::level::off);
  SPDLOG_INFO("Logging level set to off.");
#endif
}

void Runner::setGridEnabled(bool enabled) {
  setFlag(RUNNER_FLAG_GRID, enabled, flags);
}

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", display.title);
}

bool Runner::doInit() {
  return true;
}

void Runner::doPreRender(const RunnerState& state) {
  clearBuffer(background_color, clearMask);
}

void Runner::doRender(const RunnerState& state) {
  if (isGridEnabled(flags)) {
    auto proj = glm::perspective(glm::radians(45.f), display.aspectRatio, 0.1f, 100.0f);
    grid.render(proj * getViewMatrix(camera));
  }
}

void Runner::doPostRender(const RunnerState& state) {
  swapBuffers();
}

void Runner::doShutdown() {
}

void Runner::doHandleWindowSizeChanged(const int width, const int height) {
}

void Runner::doPreTick(const RunnerState& state) {
}

void Runner::doTick(const RunnerState& state) {
  if (isCameraEnabled(flags)) update(camera, state);
}

void Runner::doPostTick(const RunnerState& state) {
}

}  // namespace uinta
