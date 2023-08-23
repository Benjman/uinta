#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <entt/entity/registry.hpp>
#include <glm/vec3.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/fwd.hpp>
#include <uinta/input/state.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/scene/scene.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner {
 public:
  static constexpr flag_t GRID_ENABLED = 1 << 1;
  static constexpr flag_t RENDERING_ENABLED = 1 << 2;

  CartesianGrid grid;
  Display display;
  FileManager fileManager;
  InputState input;
  Scene scene;
  ModelManager modelManager;
  entt::registry registry;

  flags_t flags = GRID_ENABLED | RENDERING_ENABLED;

  Runner(const std::string& title, i32 argc = 0, const char** argv = nullptr) noexcept;

  ~Runner();

  i32 run();

  void handleCursorPositionChanged(const f64 xpos, const f64 ypos);
  void handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods);
  void handleMouseButtonInput(const i32 button, const u32 action, const i32 mods);
  void handleScrollInput(const f64 xoffset, const f64 yoffset);
  void handleWindowSizeChanged(const i32 width, const i32 height);

  bool isRenderingEnabled();

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
  virtual f64 getRuntime() const = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

 private:
  void tick(const RunnerState& state);
  void render(const RunnerState& state);
  void shutdown();
  bool shouldRenderFrame(f32 dt);
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
