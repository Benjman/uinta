// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <spdlog/spdlog.h>

#include <uinta/runner/display.hpp>
#include <uinta/runner/runner.hpp>

#include "./runner/display.cpp"

namespace uinta {
void setSpdlogLevel();
}  // namespace uinta

using namespace uinta;

Runner::Runner(const std::string& title, uint32_t width, uint32_t height) noexcept : display(title, width, height) {
  SPDLOG_INFO("Runner started for '{}'.", title);
  state.display = Display(title, width, height);
  setSpdlogLevel();

  force(camera.angle, 45);
  force(camera.pitch, 45);
  force(camera.dist, 5);
}

bool Runner::init() {
  spdlog::stopwatch sw;
  fileManager.init();
  fileManager.loadAll();
  if (!internalInit() || !doInit()) return false;
  startTime = getRuntime();
  SPDLOG_INFO("Completed initialization for '{}' in {} seconds.", display.title, sw.elapsed().count());
  return true;
}

int Runner::run() {
  try {
    if (!init()) {
      SPDLOG_ERROR("Failed to initialize runner! Exiting application.");
      return EXIT_FAILURE;
    }
    while (!shouldExit()) {
      pollInput();
      do {
        tick(getRuntime());
      } while (!shouldRenderFrame());
      render();
    }
    shutdown();
    return EXIT_SUCCESS;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Window '{}' encountered an error: {}", display.title, ex.what());
    throw ex;
  }
}

void Runner::tick(float runtime) {
  runtime -= startTime;
  state.delta = runtime - state.runtime;
  state.runtime = runtime;
  state.tick++;

  // SPDLOG_TRACE("tick: {}, delta: {}, runtime: {}", state.tick, state.delta, state.runtime);
  doPreTick(state);
  doTick(state);
  doPostTick(state);

  update(camera, state);

  reset(state.input);
}

void Runner::render() {
  clearBuffer();

  internalPreRender();
  doPreRender(state);

  internalRender();
  doRender(state);

  internalPostRender();
  doPostRender(state);

  swapBuffers();
}

void Runner::clearBuffer() {
  glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
  glClear(clearMask);
}

void Runner::shutdown() {
  SPDLOG_INFO("Shutdown requested for '{}'.", display.title);
  internalShutdown();
  doShutdown();
}

bool Runner::shouldRenderFrame() {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::setClearMask(const GLbitfield mask) {
  clearMask = mask;
}

void Runner::setBackground(const glm::vec3& background) {
  background_color = background;
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

void uinta::setSpdlogLevel() {
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

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", display.title);
}
