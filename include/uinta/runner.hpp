#ifndef UINTA_RUNNER_RUNNER_HPP
#define UINTA_RUNNER_RUNNER_HPP

#include <spdlog/fwd.h>

#include <glm/vec3.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <uinta/event.hpp>
#include <uinta/fwd.hpp>
#include <uinta/input.hpp>
#include <uinta/runner_state.hpp>
#include <uinta/window.hpp>

namespace uinta {

class RunnerGpuUtils;

class Runner : public EventManager {
  using SceneStack = std::list<std::unique_ptr<Scene>>;

 public:
  static constexpr flag_t IS_RUNNING = 1 << 0;

  Runner(const std::string& title, i32 argc = 0, const char** argv = nullptr, std::unique_ptr<FileManager> file_manager = nullptr,
         std::unique_ptr<RunnerGpuUtils> gpu_utils = nullptr);

  virtual ~Runner() noexcept;

  i32 run() noexcept;

  void handleCursorPositionChanged(const f64 xpos, const f64 ypos) noexcept;

  void handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods) noexcept;

  void handleMouseButtonInput(const i32 button, const u32 action, const i32 mods) noexcept;

  void handleScrollInput(const f64 xoffset, const f64 yoffset) noexcept;

  void handleWindowPosChanged(const i32 xpos, const i32 ypos) noexcept;

  void handleWindowSizeChanged(const i32 width, const i32 height) noexcept;

  virtual uinta_error_code init_gpu_context() = 0;

  uinta_error_code add_scene(std::unique_ptr<Scene> scene) noexcept;

  uinta_error_code remove_scene(const Scene* scene) noexcept;

  template <typename T>
  T* find_scene() const noexcept {
    for (const auto& scene : m_scenes)
      if (T* scene_t = dynamic_cast<T*>(scene.get())) return scene_t;
    return nullptr;
  }

  template <typename T>
  const T* find_scenec() const noexcept {
    return find_scene<T>();
  }

  virtual const TargetCamera* find_camerac() const noexcept;

  virtual TargetCamera* find_camera() const noexcept;

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

  flags_t& flags() noexcept {
    return m_flags;
  }

  u32& clear_mastk() noexcept {
    return m_clear_mask;
  }

  glm::vec3& clear_color() noexcept {
    return m_clear_color;
  }

  const RunnerState& state() const {
    return m_state;
  }

  spdlog::logger* logger() const noexcept {
    return m_logger.get();
  }

  const SceneStack& scenes() const noexcept {
    return m_scenes;
  }

  template <typename EventType>
  void add_event(EventType event_type, std::unique_ptr<const Event> event) noexcept {
    add_event(static_cast<event_t>(event_type), std::move(event));
  }

  virtual void add_event(event_t event_type, std::unique_ptr<const Event> event) noexcept override;

 protected:
  virtual f64 runtime() = 0;
  virtual void pollInput() = 0;
  virtual void swapBuffers() = 0;

 private:
  Window m_window;
  InputState m_input;
  RunnerState m_state;
  SceneStack m_scenes;
  std::mutex m_scenes_mtx;
  std::shared_ptr<spdlog::logger> m_logger;
  const std::unique_ptr<FileManager> m_file_manager;
  const std::unique_ptr<RunnerGpuUtils> m_gpu_utils;
  glm::vec3 m_clear_color = glm::vec3(38, 70, 83) / 255.0f;
  u32 m_clear_mask;
  flags_t m_flags = 0;

  void advanceState() noexcept;

  bool handleException(const UintaException& ex) noexcept;
};

class RunnerGpuUtils {
 public:
  virtual uinta_error_code init(Runner& runner) = 0;
  virtual void clear_buffer(const glm::vec3& color, u32 mask) = 0;
};

class RunnerGpuUtils_OpenGL : public RunnerGpuUtils {
 public:
  uinta_error_code init(Runner& runner) override;
  void clear_buffer(const glm::vec3& color, u32 mask) override;
};

enum class RunnerEvents : event_t {
  AspectRatioChanged,
  SceneCreated,
  SceneDestroyed,
  ScenePaused,
  SceneRunning,
};

struct AspectRatioChangeEvent : Event {
  const f64 aspect_ratio;

  AspectRatioChangeEvent(f64 at, f32 aspect_ratio) : Event(at), aspect_ratio(aspect_ratio) {
  }
};

struct SceneEvent : Event {
  const Scene* scene;

  SceneEvent(f64 at, const Scene* scene) : Event(at), scene(scene) {
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_RUNNER_HPP
