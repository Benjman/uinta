// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <exception>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <uinta/error.hpp>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

inline void clearBuffer(const glm::vec3& color, GLbitfield mask);
inline void advanceState(RunnerState& state, f64 runtime, f64& lastRuntime);
inline bool handleException(const UintaException& ex, const Runner& runner);
static spdlog::stopwatch sw;

void processArgs(Runner* runner, i32 argc, const char** argv);

Runner::Runner(const std::string& title, i32 argc, const char** argv) noexcept : m_window(title), m_scene(*this) {
  processArgs(this, argc, argv);
  initSpdlog();
  SPDLOG_INFO("Runner started for '{}'.", title);
}

i32 Runner::run() {
  try {
    if (isFlagSet(RENDERING_ENABLED, m_flags))
      if (auto error = createOpenGLContext(); error) throw UintaException(error);
    if (auto error = doInit(); error) throw UintaException(error);
    SPDLOG_INFO("Initialized '{}' in {} seconds.", m_window.title, sw.elapsed().count());
    RunnerState state;
    auto lastRuntime = runtime();
    while (!isFlagSet(STOP_RUNNING, m_flags)) {
      try {
        do {
          advanceState(state, runtime(), lastRuntime);
          tick(state);
          reset(m_input);
        } while (!shouldRenderFrame(state.delta));
        pollInput();
        if (isFlagSet(RENDERING_ENABLED, m_flags)) {
          swapBuffers();
          clearBuffer(clearColor, clearMask);
          render(state);
        }
      } catch (const UintaException& ex) {
        if (!handleException(ex, *this)) throw ex;  // handle internal game errors
      }
    }
    shutdown();
    return EXIT_SUCCESS;
  } catch (const UintaException& ex) {
    handleException(ex, *this);  // handle system boostrap or shutdown error
    throw static_cast<std::exception>(ex);
  } catch (const std::exception& ex) {
    try {
      shutdown();
    } catch (const std::exception& shutdownEx) {
      std::cerr << "Runner::shutdown() caught: " << shutdownEx.what() << "\n";
    }
    std::cerr << "Runner::run() caught: " << ex.what() << "\n";
    throw ex;
  }
}

uinta_error_code Runner::doInit() {
  if (auto error = m_file_manager.init(); error) return error;
  if (auto error = m_scene.init(*this); error) return error;
  if (auto error = m_cartesian_grid.init(m_file_manager); error) return error;
  glEnable(GL_DEPTH_TEST);
  return SUCCESS_EC;
}

void Runner::tick(const RunnerState& state) {
  doPreTick(state);
  doTick(state);
  doPostTick(state);
}

void Runner::render(const RunnerState& state) {
  doPreRender(state);
  doRender(state);
  doPostRender(state);
}

void Runner::shutdown() {
  SPDLOG_INFO("Shutdown requested for '{}'.", m_window.title);
  doShutdown();
}

bool Runner::shouldRenderFrame(f32 dt) {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::handleCursorPositionChanged(const f64 xpos, const f64 ypos) {
  mouseMoved(m_input, xpos, ypos);
}

void Runner::handleScrollInput(const f64 xoffset, const f64 yoffset) {
  mouseScrolled(m_input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods) {
  if (action == ACTION_PRESS) keyPressed(m_input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(m_input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(m_input, key, mods);
}

void Runner::handleMouseButtonInput(const i32 button, const u32 action, const i32 mods) {
  if (action == ACTION_PRESS) mouseButtonPressed(m_input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(m_input, button, mods);
  m_input.platform_flags = mods;
}

void Runner::handleWindowSizeChanged(const i32 width, const i32 height) {
  m_window.width = width;
  m_window.height = height;
  m_window.aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
  m_scene.onAspectRatioUpdate(m_window.aspectRatio);
  onWindowSizeChanged();
}

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", m_window.title);
}

void Runner::doPreRender(const RunnerState& state) {
  m_scene.startRender(state);
}

void Runner::doRender(const RunnerState& state) {
  if (isFlagSet(GRID_ENABLED, m_flags))
    m_cartesian_grid.render(getPerspectiveMatrix(m_scene.camera()) * getViewMatrix(m_scene.camera()));
}

void Runner::doPostRender(const RunnerState& state) {
}

void Runner::doShutdown() {
}

void Runner::onWindowSizeChanged() {
}

void Runner::doPreTick(const RunnerState& state) {
}

void Runner::doTick(const RunnerState& state) {
  m_scene.update(state, m_input, m_registry);
}

void Runner::doPostTick(const RunnerState& state) {
}

inline void clearBuffer(const glm::vec3& color, GLbitfield mask) {
  glClearColor(color.r, color.g, color.b, 1.0);
  glClear(mask);
}

inline void advanceState(RunnerState& state, f64 runtime, f64& lastRuntime) {
  state.tick++;
  state.runtime = runtime;
  state.delta = runtime - lastRuntime;
  lastRuntime = runtime;
}

inline bool handleException(const UintaException& ex, const Runner& runner) {
  // TODO: Once an in-game error reporting solution is implemented, this is where we can hook in to handle non-catastrophic errors
  // while the game is running. For example, if an in-game console is developed, this is where we would hook in to get the message
  // of the exception, and display it in the console.
  SPDLOG_CRITICAL(ex.what());
  return false;
}

}  // namespace uinta
