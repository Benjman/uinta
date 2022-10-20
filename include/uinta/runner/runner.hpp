#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <uinta/gl.h>

#include <glm/vec3.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/fwd.hpp>
#include <uinta/input/state.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner_flags.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

inline const glm::vec3 DEFAULT_CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

class Runner {
 public:
  CartesianGrid grid;
  Display display;
  FileManager fileManager;
  InputState input;
  RunnerState state;
  TargetCamera camera;

  flags_t flags = RUNNER_FLAG_CAMERA | RUNNER_FLAG_GRID | RUNNER_FLAG_RENDERING;

  Runner(const std::string& title, uint width, uint height) noexcept;

  ~Runner();

  int run();

 protected:
  GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  glm::vec3 background_color = DEFAULT_CLEAR_COLOR;

  virtual bool doInit();
  virtual void doPreTick(const RunnerState& state);
  virtual void doTick(const RunnerState& state);
  virtual void doPostTick(const RunnerState& state);
  virtual void doPreRender(const RunnerState& state);
  virtual void doRender(const RunnerState& state);
  virtual void doPostRender(const RunnerState& state);
  virtual void doShutdown();
  virtual void doHandleWindowSizeChanged(const int width, const int height);

  virtual bool shouldExit() = 0;
  virtual double getRuntime() const = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);

 private:
  bool init();
  void tick(float dt);
  void render();
  void shutdown();
  bool shouldRenderFrame(float dt);
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
