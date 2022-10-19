#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <uinta/camera/target_camera.hpp>
#include <uinta/flags.hpp>
#include <uinta/io.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner_flags.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner {
 public:
  CartesianGrid grid;
  Display display;
  FileManager fileManager;
  RunnerState state;
  TargetCamera camera;

  glm::vec3 background_color;
  GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  flags_t flags;

  Runner(const std::string& title, uint32_t width, uint32_t height) noexcept;

  ~Runner();

  int run();

  virtual double getRuntime() const = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;
  virtual bool shouldExit() = 0;
  virtual void doHandleWindowSizeChanged(const int width, const int height);

  /** life cycle **/
  virtual bool doInit();
  virtual void doPreTick(const RunnerState& state);
  virtual void doTick(const RunnerState& state);
  virtual void doPostTick(const RunnerState& state);
  virtual void doPreRender(const RunnerState& state);
  virtual void doRender(const RunnerState& state);
  virtual void doPostRender(const RunnerState& state);
  virtual void doShutdown();
  /** life cycle end **/

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);
  bool init();
  void render();
  void setBackground(const glm::vec3& background);
  void setClearMask(const GLbitfield mask);
  void setGridEnabled(bool enabled);
  bool shouldRenderFrame(float dt);
  void shutdown();
  void tick(float dt);
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
