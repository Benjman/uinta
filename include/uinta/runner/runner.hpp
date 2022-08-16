#ifndef UINTA_APP_RUNNER_HPP
#define UINTA_APP_RUNNER_HPP

#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <string>

#include "./runner_state.hpp"
#include "uinta/io.hpp"

namespace uinta {

const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

struct Runner {
  Display display;
  FileManager fileManager;
  RunnerState state;
  logger_t logger;

  Runner(const std::string& title, unsigned int width, unsigned int height) noexcept;

  bool init();
  bool initIO();
  int run();
  void tick(float dt);
  void render();
  void shutdown();

  bool shouldRenderFrame();

  void setClearMask(const GLbitfield mask);
  void setBackground(const glm::vec3& background);

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);

 protected:
  virtual bool internalInit() = 0;

  virtual bool doInit() { return true; }
  virtual void doInitFiles() {}

  virtual void internalPreTick() {}
  virtual void doPreTick(const RunnerState& state) {}
  virtual void internalTick() {}
  virtual void doTick(const RunnerState& state) {}
  virtual void internalPostTick() {}
  virtual void doPostTick(const RunnerState& state) {}

  virtual void internalPreRender() {}
  virtual void doPreRender() {}
  virtual void internalRender() {}
  virtual void doRender() {}
  virtual void internalPostRender() {}
  virtual void doPostRender() {}
  virtual void swapBuffers() = 0;

  virtual void internalShutdown() = 0;
  virtual void doShutdown() {}

  virtual void doHandleWindowSizeChanged(const int width, const int height) {}
  virtual bool shouldExit() = 0;
  virtual double getRuntime() = 0;
  virtual void pollInput() = 0;

 private:
  GLbitfield clear_mask = GL_COLOR_BUFFER_BIT;
  glm::vec3 background_color = DEFAULT_CLEAR_COLOR;
};

}  // namespace uinta

#endif  // UINTA_APP_RUNNER_HPP
