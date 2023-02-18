#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <uinta/gl.h>

#include <glm/vec3.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/fwd.hpp>
#include <uinta/input/state.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/runner/args.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/runner/runner_flags.hpp>
#include <uinta/runner/scene.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner {
 public:
  CartesianGrid grid;
  Display display;
  FileManager fileManager;
  InputState input;
  TargetCamera camera;
  Scene scene;
  ModelManager modelManager;

  flags_t flags = RUNNER_FLAG_CAMERA | RUNNER_FLAG_GRID | RUNNER_FLAG_RENDERING;

  Runner(const std::string& title, int argc = 0, const char** argv = nullptr) noexcept;

  ~Runner();

  int run();

  void handleCursorPositionChanged(const double xpos, const double ypos);
  void handleKeyInput(const input_key_t key, const int scancode, const int action, const int mods);
  void handleMouseButtonInput(const int button, const int action, const int mods);
  void handleScrollInput(const double xoffset, const double yoffset);
  void handleWindowSizeChanged(const int width, const int height);

 protected:
  GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  glm::vec3 clearColor = glm::vec3(38, 70, 83) / 255.0f;

  virtual bool doInit();
  virtual void doPreTick(const RunnerState& state);
  virtual void doTick(const RunnerState& state);
  virtual void doPostTick(const RunnerState& state);
  virtual void doPreRender(const RunnerState& state);
  virtual void doRender(const RunnerState& state);
  virtual void doPostRender(const RunnerState& state);
  virtual void doShutdown();
  virtual void onWindowSizeChanged();

  virtual bool createOpenGLContext() = 0;
  virtual bool shouldExit() = 0;
  virtual double getRuntime() const = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

 private:
  void tick(const RunnerState& state);
  void render(const RunnerState& state);
  void shutdown();
  bool shouldRenderFrame(float dt);
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
