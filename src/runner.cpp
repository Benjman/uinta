// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <glm/mat4x4.hpp>
#include <iostream>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/args.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

inline void clearBuffer(const glm::vec3& color, GLbitfield mask);
inline void advanceState(RunnerState& state, double runtime, double& lastRuntime);

Runner::Runner(const std::string& title, int argc, const char** argv) noexcept : display(title) {
  processArgs(this, argc, argv);
  initSpdlog();
  SPDLOG_INFO("Runner started for '{}'.", title);
}

int Runner::run() {
  try {
    if (isRenderingEnabled(flags) && !createOpenGLContext()) return false;
    if (!doInit()) {
      SPDLOG_ERROR("Failed to initialize runner! Exiting application.");
      return EXIT_FAILURE;
    }
    RunnerState state;
    auto lastRuntime = getRuntime();
    while (!shouldExit()) {
      do {
        advanceState(state, getRuntime(), lastRuntime);
        tick(state);
        reset(input);
      } while (!shouldRenderFrame(state.delta));
      pollInput();
      if (isRenderingEnabled(flags)) {
        swapBuffers();
        clearBuffer(clearColor, clearMask);
        render(state);
      }
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

bool Runner::doInit() {
  spdlog::stopwatch sw{};
  if (!fileManager.init()) return false;
  if (!scene.init(this)) return false;
  if (isGridEnabled(flags) && !grid.init(fileManager)) return false;
  SPDLOG_INFO("Initialized '{}' in {} seconds.", display.title, sw.elapsed().count());
  if (isCameraEnabled(flags)) {
    force(camera.angle, 45);
    force(camera.pitch, 45);
    force(camera.dist, 5);
  }
  return true;
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
  SPDLOG_INFO("Shutdown requested for '{}'.", display.title);
  doShutdown();
}

bool Runner::shouldRenderFrame(float dt) {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::handleCursorPositionChanged(const double xpos, const double ypos) {
  mouseMoved(input, xpos, ypos);
}

void Runner::handleScrollInput(const double xoffset, const double yoffset) {
  mouseScrolled(input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
  if (action == ACTION_PRESS) keyPressed(input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(input, key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
  if (action == ACTION_PRESS) mouseButtonPressed(input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(input, button, mods);
  input.platform_flags = mods;
}

void Runner::handleWindowSizeChanged(const int width, const int height) {
  display.width = width;
  display.height = height;
  display.aspectRatio = (float)width / (float)height;
  onWindowSizeChanged();
}

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", display.title);
}

void Runner::doPreRender(const RunnerState& state) {
}

void Runner::doRender(const RunnerState& state) {
  if (isGridEnabled(flags)) grid.render(getPerspectiveMatrix(camera, display) * getViewMatrix(camera));
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
  if (isCameraEnabled(flags)) update(camera, state, input);
}

void Runner::doPostTick(const RunnerState& state) {
}

inline void clearBuffer(const glm::vec3& color, GLbitfield mask) {
  glClearColor(color.r, color.g, color.b, 1.0);
  glClear(mask);
}

inline void advanceState(RunnerState& state, double runtime, double& lastRuntime) {
  state.tick++;
  state.runtime = runtime;
  state.delta = runtime - lastRuntime;
  lastRuntime = runtime;
}

}  // namespace uinta
