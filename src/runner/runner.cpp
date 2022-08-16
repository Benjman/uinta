// clang-format off
#include <glad/glad.h>
// clang-format on

#include "uinta/runner/runner.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace uinta {
void setSpdlogLevel();
}  // namespace uinta

using namespace uinta;

Runner::Runner(const std::string& title, unsigned int width, unsigned int height) noexcept : display(title, width, height) {
  state.display = Display(title, width, height);
  setSpdlogLevel();
  logger = spdlog::stdout_color_mt("Runner");
}

bool Runner::init() { return initIO() && internalInit() && doInit(); }

bool Runner::initIO() {
  fileManager.init();
  doInitFiles();
  fileManager.loadAll();
  return true;
}

int Runner::run() {
  try {
    if (!init()) {
      SPDLOG_LOGGER_ERROR(logger, "Failed to initialize runner! Exiting application.");
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
    SPDLOG_LOGGER_ERROR(logger, "Window '{}' encountered an error: {}", display.title, ex.what());
    return EXIT_FAILURE;
  }
}

void Runner::tick(float runtime) {
  state.delta = runtime - state.runtime;
  state.runtime = runtime;
  state.tick++;
  doPreTick(state);
  doTick(state);
  doPostTick(state);
  state.input.reset();
}

void Runner::render() {
  glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
  glClear(clear_mask);

  internalPreRender();
  doPreRender();

  internalRender();
  doRender();

  doPostRender();
  internalPostRender();

  swapBuffers();
}

void Runner::shutdown() {
  internalShutdown();
  doShutdown();
}

bool Runner::shouldRenderFrame() {
  // TODO Runner should have a `targetFps`, and this method will return true when a frame needs to be rendered based on `runtime -
  // lastFrame >= targetFps`
  //      See https://github.com/Benjman/renderer/blob/main/src/core/src/runner.cpp#L46
  return true;
}

void Runner::setClearMask(const GLbitfield mask) { clear_mask = mask; }

void Runner::setBackground(const glm::vec3& background) { background_color = background; }

void Runner::handleCursorPositionChanged(const double xpos, const double ypos) {
  state.input.cursordx = xpos - state.input.cursorx;
  state.input.cursordy = ypos - state.input.cursory;
  state.input.cursorx = xpos;
  state.input.cursory = ypos;
}

void Runner::handleScrollInput(const double xoffset, const double yoffset) {
  SPDLOG_LOGGER_DEBUG(logger, "Runner::handleScrollInput - Mouse scroll (%+.1f, %+.1f)", xoffset, yoffset);
  state.input.scrolldx = xoffset;
  state.input.scrolldy = yoffset;
}

void Runner::handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods) {
  SPDLOG_LOGGER_DEBUG(logger, "Runner::handleKeyInput - Key {} event: {}{}", getActionStr(action), getModsStr(mods),
                      getKeyStr(key));
  if (action == ACTION_PRESS) state.input.keyPressed(key, mods);
  if (action == ACTION_RELEASE) state.input.keyReleased(key, mods);
  if (action == ACTION_REPEAT) state.input.keyRepeated(key, mods);
}

void Runner::handleMouseButtonInput(const int button, const int action, const int mods) {
  SPDLOG_LOGGER_DEBUG(logger, "Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
  if (action == ACTION_PRESS) state.input.mouseButtonPressed(button, mods);
  if (action == ACTION_RELEASE) state.input.mouseButtonReleased(button, mods);
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
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_DEBUG
  spdlog::set_level(spdlog::level::debug);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_INFO
  spdlog::set_level(spdlog::level::info);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_WARN
  spdlog::set_level(spdlog::level::warn);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_ERROR
  spdlog::set_level(spdlog::level::err);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_CRITICAL
  spdlog::set_level(spdlog::level::critical);
#endif

#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_OFF
  spdlog::set_level(spdlog::level::off);
#endif
}
