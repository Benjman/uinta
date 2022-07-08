#ifndef UINTA_APP_RUNNER_HPP
#define UINTA_APP_RUNNER_HPP

#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/resource.hpp>

#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <string>

namespace uinta {

const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

struct Display {
  std::string title;
  unsigned int width;
  unsigned int height;
  float aspectRatio;

  Display() : Display("", 0, 0) {}

  Display(const std::string &title, const unsigned int width, const unsigned int height) noexcept;
};

struct RunnerState final {
  /// number of times the tick cycle has been executed
  unsigned int tick;

  /// time in seconds since last tick cycle
  float delta;

  /// time in secinds the runner has been ticking
  float runtime;

  InputState input;
  Display display;

  RunnerState() {
    tick    = 0;
    delta   = 0.0;
    runtime = 0.0;
    input   = InputState();
    display = Display();
  }
};

struct Runner {
  Display display;
  RunnerState state;
  logger_t logger;
  resource::ResourceManager resources;

  Runner(const std::string &title, unsigned int width, unsigned int height) noexcept;

  bool init();
  int run();
  void tick(float dt);
  void render();
  void shutdown();

  bool shouldRenderFrame();

  void setClearMask(const GLbitfield mask);
  void setBackground(const glm::vec3 &background);

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);

  virtual bool doInit() { return true; }

  virtual void doPreTick(const RunnerState &state) {}
  virtual void doTick(const RunnerState &state) {}
  virtual void doPostTick(const RunnerState &state) {}

  virtual void doPreRender() {}
  virtual void doRender() {}
  virtual void doPostRender() {}

  virtual void doShutdown() {}

protected:
  virtual bool internalInit() = 0;

  virtual bool shouldExit()   = 0;
  virtual double getRuntime() = 0;
  virtual void pollInput()    = 0;
  virtual void swapBuffers()  = 0;

  virtual void internalPreTick() {}
  virtual void internalTick() {}
  virtual void internalPostTick() {}

  virtual void internalPreRender() {}
  virtual void internalRender() {}
  virtual void internalPostRender() {}

  virtual void internalShutdown() = 0;

private:
  GLbitfield clear_mask      = GL_COLOR_BUFFER_BIT;
  glm::vec3 background_color = DEFAULT_CLEAR_COLOR;
};

} // namespace uinta

#endif // UINTA_APP_RUNNER_HPP
