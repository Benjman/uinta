// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <glm/mat4x4.hpp>
#include <iostream>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

inline void clearBuffer(const glm::vec3& color, GLbitfield mask);
inline void advanceState(RunnerState& state, f64 runtime, f64& lastRuntime);
static spdlog::stopwatch sw;

void processArgs(Runner* runner, i32 argc, const char** argv);

Runner::Runner(const std::string& title, i32 argc, const char** argv) noexcept : display(title) {
  processArgs(this, argc, argv);
  initSpdlog();
  SPDLOG_INFO("Runner started for '{}'.", title);
}

i32 Runner::run() {
  try {
    if (isFlagSet(RENDERING_ENABLED, flags) && !createOpenGLContext()) return false;
    if (!doInit()) {
      SPDLOG_ERROR("Failed to initialize runner! Exiting application.");
      return EXIT_FAILURE;
    }
    SPDLOG_INFO("Initialized '{}' in {} seconds.", display.title, sw.elapsed().count());
    RunnerState state;
    auto lastRuntime = getRuntime();
    while (!shouldExit()) {
      do {
        advanceState(state, getRuntime(), lastRuntime);
        tick(state);
        reset(input);
      } while (!shouldRenderFrame(state.delta));
      pollInput();
      if (isFlagSet(RENDERING_ENABLED, flags)) {
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
  if (!fileManager.init()) return false;
  if (!scene.init(this)) return false;
  if (!grid.init(fileManager)) return false;
  glEnable(GL_DEPTH_TEST);
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

bool Runner::shouldRenderFrame(f32 dt) {
  // TODO Runner should have a `targetFps`; this method returns true when `runtime - lastFrame >= targetFps`
  //
  // See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::handleCursorPositionChanged(const f64 xpos, const f64 ypos) {
  mouseMoved(input, xpos, ypos);
}

void Runner::handleScrollInput(const f64 xoffset, const f64 yoffset) {
  mouseScrolled(input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods) {
  if (action == ACTION_PRESS) keyPressed(input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(input, key, mods);
}

void Runner::handleMouseButtonInput(const i32 button, const u32 action, const i32 mods) {
  if (action == ACTION_PRESS) mouseButtonPressed(input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(input, button, mods);
  input.platform_flags = mods;
}

void Runner::handleWindowSizeChanged(const i32 width, const i32 height) {
  display.width = width;
  display.height = height;
  display.aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
  scene.camera.config.aspectRatio = display.aspectRatio;
  onWindowSizeChanged();
}

Runner::~Runner() {
  SPDLOG_INFO("Tearing down '{}'.", display.title);
}

void Runner::doPreRender(const RunnerState& state) {
  scene.startRender(state);
}

void Runner::doRender(const RunnerState& state) {
  if (isFlagSet(GRID_ENABLED, flags)) grid.render(getPerspectiveMatrix(scene.camera) * getViewMatrix(scene.camera));
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
  scene.update(state, input, registry);
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

}  // namespace uinta
