#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <uinta/gl.h>

#include <glm/vec3.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/fwd.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner_flags.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

inline const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

class Runner {
 public:
  Display display;
  FileManager fileManager;
  RunnerState state;
  float startTime;
  flags_t flags;

  Runner(const std::string& title, uint width, uint height) noexcept;

  ~Runner();

  int run();

 protected:
  GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  glm::vec3 background_color = DEFAULT_CLEAR_COLOR;

  virtual bool internalInit() {
    return true;
  }
  virtual void internalPreTick() {
  }
  virtual bool doInit();
  virtual void doPreTick(const RunnerState& state);
  virtual void doTick(const RunnerState& state);
  virtual void doPostTick(const RunnerState& state);
  virtual void internalTick() {
  }
  virtual void internalPostTick() {
  }
  virtual void internalPreRender() {
  }
  virtual void internalRender() {
  }
  virtual void internalPostRender() {
  }
  virtual void doPreRender(const RunnerState& state);
  virtual void doRender(const RunnerState& state);
  virtual void doPostRender(const RunnerState& state);
  virtual void doShutdown();
  virtual void doHandleWindowSizeChanged(const int width, const int height);
  virtual void internalShutdown() {
  }

  virtual bool shouldExit() = 0;
  virtual double getRuntime() = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);

 private:
  void clearBuffer();
  bool init();
  void tick(float dt);
  void render();
  void shutdown();
  bool shouldRenderFrame();
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
