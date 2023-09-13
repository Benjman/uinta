// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <spdlog/sinks/stdout_color_sinks.h>

#include <exception>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <uinta/error.hpp>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

static spdlog::stopwatch sw;

void processArgs(Runner* runner, i32 argc, const char** argv);

Runner::Runner(const std::string& title, i32 argc, const char** argv, RunnerDependencies dependencies) noexcept
    : m_logger(spdlog::stdout_color_mt(title)),
      m_window(title),
      m_scene(*this, std::move(dependencies.scene)),
      m_file_manager(std::move(dependencies.file_manager)),
      m_gpu_utils(std::move(dependencies.gpu_utils)) {
  assert(m_file_manager && "File manager must be initialized!");
  assert(m_gpu_utils && "GPU Utilities must be initialized!");
  processArgs(this, argc, argv);
  initSpdlog();
  SPDLOG_LOGGER_INFO(m_logger, "Runner started for '{}'.", title);
}

i32 Runner::run() {
  try {
    if (isFlagSet(RENDERING_ENABLED, m_flags))
      if (auto error = createOpenGLContext(); error) throw UintaException(error);
    if (auto error = doInit(); error) throw UintaException(error);
    SPDLOG_LOGGER_INFO(m_logger, "Initialized '{}' in {} seconds.", m_window.title, sw.elapsed().count());
    while (!isFlagSet(STOP_RUNNING, m_flags)) {
      try {
        do {
          advanceState();
          tick();
          reset(m_input);
        } while (!shouldRenderFrame(m_state.delta));
        pollInput();
        if (isFlagSet(RENDERING_ENABLED, m_flags)) {
          swapBuffers();
          m_gpu_utils->clear_buffer(clearColor, clearMask);
          render();
        }
      } catch (const UintaException& ex) {
        if (!handleException(ex)) throw ex;  // handle internal game errors
      }
    }
    shutdown();
    return EXIT_SUCCESS;
  } catch (const UintaException& ex) {
    handleException(ex);  // handle system boostrap or shutdown error
    throw ex;
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
  if (auto error = m_file_manager->init(); error) return error;
  if (auto error = m_scene.init(); error) return error;
  if (auto error = m_gpu_utils->init(); error) return error;
  return SUCCESS_EC;
}

void Runner::tick() {
  doPreTick();
  doTick();
  doPostTick();
}

void Runner::render() {
  doPreRender();
  doRender();
  doPostRender();
}

void Runner::shutdown() {
  SPDLOG_LOGGER_INFO(m_logger, "Shutdown requested for '{}'.", m_window.title);
  doShutdown();
}

bool Runner::shouldRenderFrame(f32 dt) {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::advanceState() {
  m_state.tick++;
  m_state.delta = runtime() - m_state.runtime;
  m_state.runtime += m_state.delta;
}

void Runner::handleCursorPositionChanged(const f64 xpos, const f64 ypos) {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse position event x:{} y:{}", xpos, ypos);
  mouseMoved(m_input, xpos, ypos);
}

void Runner::handleScrollInput(const f64 xoffset, const f64 yoffset) {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse scroll event x:{} y:{}", xoffset, yoffset);
  mouseScrolled(m_input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods) {
  SPDLOG_LOGGER_TRACE(m_logger, "Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
  if (action == ACTION_PRESS) keyPressed(m_input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(m_input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(m_input, key, mods);
}

void Runner::handleMouseButtonInput(const i32 button, const u32 action, const i32 mods) {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
  if (action == ACTION_PRESS) mouseButtonPressed(m_input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(m_input, button, mods);
  m_input.platform_flags = mods;
}

void Runner::handleWindowPosChanged(const i32 xpos, const i32 ypos) {
  SPDLOG_LOGGER_TRACE(m_logger, "Window position updated: {}x{}.", xpos, ypos);
}

void Runner::handleWindowSizeChanged(const i32 width, const i32 height) {
  SPDLOG_LOGGER_DEBUG(m_logger, "Window size updated: {}x{}.", width, height);
  const auto orig_width = m_window.width;
  const auto orig_height = m_window.width;
  auto win = window();
  win.width = width;
  win.height = height;
  m_window = win;
  if (orig_width != m_window.width || orig_height != m_window.height) {
    m_scene.onAspectRatioUpdate(m_window.aspect_ratio);
  }
}

Runner::~Runner() {
  SPDLOG_LOGGER_INFO(m_logger, "Tearing down '{}'.", m_window.title);
}

void Runner::doPreRender() {
}

void Runner::doRender() {
  m_scene.render(m_state);
}

void Runner::doPostRender() {
}

void Runner::doShutdown() {
}

void Runner::doPreTick() {
  m_scene.preTick(m_state, m_input);
}

void Runner::doTick() {
  m_scene.tick(m_state, m_input);
}

void Runner::doPostTick() {
  m_scene.postTick(m_state, m_input);
}

bool Runner::handleException(const UintaException& ex) {
  // TODO: Once an in-game error reporting solution is implemented, this is where we can hook in to handle non-catastrophic errors
  // while the game is running. For example, if an in-game console is developed, this is where we would hook in to get the message
  // of the exception, and display it in the console.
  SPDLOG_LOGGER_CRITICAL(m_logger, ex.what());
  return false;
}

uinta_error_code RunnerGpuUtils_OpenGL::init() {
  glEnable(GL_DEPTH_TEST);
  return SUCCESS_EC;
}

void RunnerGpuUtils_OpenGL::clear_buffer(const glm::vec3& color, GLbitfield mask) {
  glClearColor(color.r, color.g, color.b, 1.0);
  glClear(mask);
}

}  // namespace uinta
