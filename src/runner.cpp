// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <iostream>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>

using namespace uinta;

Runner::Runner(const std::string& title, uint32_t width, uint32_t height) noexcept : display(title, width, height) {
  initSpdlog();
  SPDLOG_INFO("Runner started for '{}'.", title);
  state.display = Display(title, width, height);
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
    try {
      shutdown();
    } catch (const std::exception& shutdownEx) {
      std::cerr << "Runner::shutdown() caught: " << shutdownEx.what() << "\n";
    }
    std::cerr << "Runner::run() caught: " << ex.what() << "\n";
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

void Runner::handleCursorPositionChanged(const double xpos, const double ypos) {
  mouseMoved(state.input, xpos, ypos);
}

void Runner::handleScrollInput(const double xoffset, const double yoffset) {
  mouseScrolled(state.input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
  if (action == ACTION_PRESS) keyPressed(state.input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(state.input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(state.input, key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
  if (action == ACTION_PRESS) mouseButtonPressed(state.input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(state.input, button, mods);
  state.input.platform_flags = mods;
}

void Runner::handleWindowSizeChanged(const int width, const int height) {
  state.display.width = width;
  state.display.height = height;
  doHandleWindowSizeChanged(width, height);
}

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", display.title);
}

bool Runner::doInit() {
  return true;
}

void Runner::doPreRender(const RunnerState& state) {
}

void Runner::doRender(const RunnerState& state) {
}

void Runner::doPostRender(const RunnerState& state) {
}

void Runner::doShutdown() {
}

void Runner::doHandleWindowSizeChanged(const int width, const int height) {
  display.width = width;
  display.height = height;
  display.aspectRatio = (float)width / (float)height;
}

void Runner::doPreTick(const RunnerState& state) {
}

void Runner::doTick(const RunnerState& state) {
}

void Runner::doPostTick(const RunnerState& state) {
}
