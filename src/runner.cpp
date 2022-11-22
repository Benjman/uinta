// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>

namespace uinta {

void clearBuffer(const glm::vec3& color, GLbitfield mask);

}  // namespace uinta

using namespace uinta;

Runner::Runner(const std::string& title, uint width, uint height) noexcept : display(title, width, height) {
  initSpdlog();
  SPDLOG_INFO("Runner started for '{}'.", title);
  state.display = Display(title, width, height);

  if (isCameraEnabled(flags)) {
    force(camera.angle, 45);
    force(camera.pitch, 45);
    force(camera.dist, 5);
  }
}

bool Runner::init() {
  spdlog::stopwatch sw;
  fileManager.init();
  fileManager.loadAll();
  if (!doInit()) return false;
  if (isGridEnabled(flags) && !grid.init(fileManager)) return false;
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
        reset(state.input);
      } while (!shouldRenderFrame(state.delta));
      pollInput();
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

inline void uinta::clearBuffer(const glm::vec3& color, GLbitfield mask) {
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
  display.width = width;
  display.height = height;
  display.aspectRatio = (float)width / (float)height;
}

void Runner::doPreTick(const RunnerState& state) {
}

void Runner::doTick(const RunnerState& state) {
  if (isCameraEnabled(flags)) update(camera, state, state.input);
}

void Runner::doPostTick(const RunnerState& state) {
}
