#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <spdlog/fwd.h>

#include <entt/entity/registry.hpp>
#include <glm/vec3.hpp>
#include <uinta/exception.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/input/state.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/runner/dependencies.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/runner/window.hpp>
#include <uinta/scene/scene.hpp>

namespace uinta {

class Runner {
 public:
  static constexpr flag_t RENDERING_ENABLED = 1 << 0;
  static constexpr flag_t IS_RUNNING = 1 << 1;

  Runner(const std::string& title, i32 argc = 0, const char** argv = nullptr, RunnerDependencies dependencies = {}) noexcept;

  ~Runner();

  i32 run();

  void handleCursorPositionChanged(const f64 xpos, const f64 ypos);
  void handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods);
  void handleMouseButtonInput(const i32 button, const u32 action, const i32 mods);
  void handleScrollInput(const f64 xoffset, const f64 yoffset);
  void handleWindowPosChanged(const i32 xpos, const i32 ypos);
  void handleWindowSizeChanged(const i32 width, const i32 height);

  const Window& window() const noexcept {
    return m_window;
  }

  void window(const Window& v) noexcept {
    m_window = v;
  }

  FileManager& file_manager() noexcept {
    return *m_file_manager;
  }

  InputState& input() noexcept {
    return m_input;
  }

  void input(const InputState& v) {
    m_input = v;
  }

  Scene& scene() noexcept {
    return m_scene;
  }

  ModelManager& model_manager() noexcept {
    return m_model_manager;
  }

  void model_manager(const ModelManager& v) {
    m_model_manager = v;
  }

  flags_t& flags() noexcept {
    return m_flags;
  }

  const RunnerState& state() const {
    return m_state;
  }

  const spdlog::logger* logger() const noexcept {
    return m_logger.get();
  }

 protected:
  RunnerState m_state;
  const std::shared_ptr<spdlog::logger> m_logger;
  GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  glm::vec3 clearColor = glm::vec3(38, 70, 83) / 255.0f;
  flags_t m_flags = RENDERING_ENABLED | IS_RUNNING;

  virtual uinta_error_code doInit();
  virtual void doPreTick();
  virtual void doTick();
  virtual void doPostTick();
  virtual void doPreRender();
  virtual void doRender();
  virtual void doPostRender();
  virtual void doShutdown();

  virtual uinta_error_code createOpenGLContext() = 0;
  virtual f64 runtime() = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

 private:
  Window m_window;
  InputState m_input;
  Scene m_scene;
  ModelManager m_model_manager;
  std::unique_ptr<FileManager> m_file_manager;
  std::unique_ptr<RunnerGpuUtils> m_gpu_utils;

  void tick();
  void render();
  void shutdown();
  bool shouldRenderFrame(f32 dt);
  void advanceState();
  bool handleException(const UintaException& ex);
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
