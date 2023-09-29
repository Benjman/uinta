// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <spdlog/sinks/stdout_color_sinks.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/scene.hpp>

namespace uinta {

enum class error {
  SceneInvalid = 100,
  SceneNotFound = 101,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::SceneInvalid), "Invalid scene."},
    {static_cast<uinta_error_code_t>(error::SceneNotFound), "Failed to locate scene in stack."},
};

UINTA_ERROR_FRAMEWORK(Runner, errorMessages);

void processArgs(Runner* runner, i32 argc, const char** argv);

Runner::Runner(const std::string& title, i32 argc, const char** argv, std::unique_ptr<FileManager> file_manager,
               std::unique_ptr<RunnerGpuUtils> gpu_utils)
    : m_window(title),
      m_logger(spdlog::stdout_color_mt(title)),
      m_file_manager(std::move(file_manager)),
      m_gpu_utils(std::move(gpu_utils)) {
  assert(m_file_manager && "File manager must be initialized!");
  assert(m_gpu_utils && "GPU Utilities must be initialized!");
  processArgs(this, argc, argv);
  initSpdlog();
  if (auto error = m_file_manager->init(*this); error) throw UintaException(error);
  SPDLOG_LOGGER_INFO(m_logger, "Runner started for '{}'.", title);
}

Runner::~Runner() noexcept = default;

i32 Runner::run() noexcept {
  spdlog::stopwatch sw;
  if (auto error = m_gpu_utils->init(*this); error) {
    SPDLOG_LOGGER_CRITICAL(m_logger, "Error attempting to initialize GPU utilites: ", error.message());
    return error.value();
  }
  for (auto itr = m_scenes.cbegin(); itr != m_scenes.cend();) {
    auto* scene = itr->get();
    if (auto error = scene->init(); error) {
      SPDLOG_LOGGER_ERROR(scene->logger(), "Error in initialization: {}", error.message());
      (void)scene->transition(Scene::State::Destroyed);
      itr = m_scenes.erase(itr);
    } else
      itr++;
  }
  SPDLOG_LOGGER_INFO(m_logger, "Initialized in {} seconds.", sw.elapsed().count());
  while (isFlagSet(IS_RUNNING, m_flags)) {
    advanceState();
    for (auto itr = m_scenes.cbegin(); itr != m_scenes.cend();) {
      auto* scene = itr->get();
      try {
        switch (scene->state()) {
          case Scene::State::Created:
            if (auto error = scene->init(); error) {
              SPDLOG_LOGGER_ERROR(scene->logger(), "Error in initialization: {}", error.message());
              (void)scene->transition(Scene::State::Destroyed);
              itr = m_scenes.erase(itr);
            } else
              itr++;
            continue;
          case Scene::State::Running:
            scene->pre_tick(m_state, m_input);
            scene->tick(m_state, m_input);
            scene->post_tick(m_state, m_input);
            break;
          case Scene::State::Destroyed:
          case Scene::State::Paused:
            break;
        }
        itr++;
      } catch (const UintaException& ex) {
        SPDLOG_LOGGER_ERROR(scene->logger(), "Exception thrown in advance stage: {}", ex.what());
      }
    }
    reset(m_input);
    pollInput();
    swapBuffers();
    m_gpu_utils->clear_buffer(m_clear_color, m_clear_mask);
    for (auto& scene : m_scenes) {
      if (scene->state() == Scene::State::Running) {
        try {
          scene->pre_render(m_state);
          scene->render(m_state);
          scene->post_render(m_state);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "Exception thrown in render stage: {}", ex.what());
        }
      }
    }
  }
  SPDLOG_LOGGER_INFO(m_logger, "Shutting down.");
  for (auto& scene : m_scenes) {
    if (Scene::State::Destroyed == scene->state()) continue;
    try {
      scene->shutdown();
    } catch (const std::exception& ex) {
      SPDLOG_LOGGER_ERROR(scene->logger(), "Exception thrown in shutdown stage: {}", ex.what());
    }
  }
  return EXIT_SUCCESS;
}

void Runner::advanceState() noexcept {
  m_state.tick++;
  m_state.delta = runtime() - m_state.runtime;
  m_state.runtime += m_state.delta;
}

void Runner::handleCursorPositionChanged(const f64 xpos, const f64 ypos) noexcept {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse position event x:{} y:{}", xpos, ypos);
  mouseMoved(m_input, xpos, ypos);
}

void Runner::handleScrollInput(const f64 xoffset, const f64 yoffset) noexcept {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse scroll event x:{} y:{}", xoffset, yoffset);
  mouseScrolled(m_input, xoffset, yoffset);
}

void Runner::handleKeyInput(const input_key_t key, const i32 scancode, const u32 action, const i32 mods) noexcept {
  SPDLOG_LOGGER_TRACE(m_logger, "Key event: {} {}{}", getActionStr(action), getModsStr(mods), getKeyStr(key));
  if (action == ACTION_PRESS) keyPressed(m_input, key, mods);
  if (action == ACTION_RELEASE) keyReleased(m_input, key, mods);
  if (action == ACTION_REPEAT) keyRepeated(m_input, key, mods);
}

void Runner::handleMouseButtonInput(const i32 button, const u32 action, const i32 mods) noexcept {
  SPDLOG_LOGGER_TRACE(m_logger, "Mouse {} event: {}{}", getActionStr(action), getModsStr(mods), getMouseButtonStr(button));
  if (action == ACTION_PRESS) mouseButtonPressed(m_input, button, mods);
  if (action == ACTION_RELEASE) mouseButtonReleased(m_input, button, mods);
  m_input.platform_flags = mods;
}

void Runner::handleWindowPosChanged(const i32 xpos, const i32 ypos) noexcept {
  SPDLOG_LOGGER_TRACE(m_logger, "Window position updated: {}x{}.", xpos, ypos);
}

void Runner::handleWindowSizeChanged(const i32 width, const i32 height) noexcept {
  // TODO: Once the event system is running, broadcast window size changed event
  // SPDLOG_LOGGER_DEBUG(m_logger, "Window size updated: {}x{}.", width, height);
  // const auto orig_width = m_window.width;
  // const auto orig_height = m_window.height;
  // auto win = window();
  // win.width = width;
  // win.height = height;
  // m_window = win;
  // if (orig_width != m_window.width || orig_height != m_window.height) {
  //   m_scene->onAspectRatioUpdate(m_window.aspect_ratio);
  // }
}

bool Runner::handleException(const UintaException& ex) noexcept {
  // TODO: Once an in-game error reporting solution is implemented, this is where we can hook in to handle non-catastrophic errors
  // while the game is running. For example, if an in-game console is developed, this is where we would hook in to get the message
  // of the exception, and display it in the console.
  SPDLOG_LOGGER_CRITICAL(m_logger, ex.what());
  return false;
}

uinta_error_code Runner::add_scene(std::unique_ptr<Scene> scene) noexcept {
  if (!scene) return make_error(error::SceneInvalid);
  auto itr = m_scenes.begin();
  for (; itr != m_scenes.end(); ++itr) {
    if (itr->get()->layer() > scene->layer()) {
      break;
    }
  }
  std::lock_guard<std::mutex> lock(m_scenes_mtx);
  itr = m_scenes.insert(itr, std::move(scene));
  SPDLOG_LOGGER_INFO(m_logger, "{} scene '{}' added.", to_string(itr->get()->layer()), itr->get()->name());
  return SUCCESS_EC;
}

uinta_error_code Runner::remove_scene(const Scene* scene) noexcept {
  if (!scene) return make_error(error::SceneInvalid);
  std::lock_guard<std::mutex> lock(m_scenes_mtx);
  for (auto itr = m_scenes.cbegin(); itr != m_scenes.cend();) {
    if (itr->get() == scene) {
      if (auto error = itr->get()->transition(Scene::State::Destroyed); error) return error;
      SPDLOG_LOGGER_INFO(m_logger, "Scene '{}' removed.", itr->get()->name());
      m_scenes.erase(itr);
      break;
    } else
      itr++;
  }
  return SUCCESS_EC;
}

const TargetCamera* Runner::find_camerac() const noexcept {
  return find_camera();
}

TargetCamera* Runner::find_camera() const noexcept {
  for (auto& scene : m_scenes)
    if (auto* camera = scene->camera(); camera) return camera;
  return nullptr;
}

uinta_error_code RunnerGpuUtils_OpenGL::init(Runner& runner) {
  if (auto error = runner.init_gpu_context(); error) throw UintaException(error);
  glEnable(GL_DEPTH_TEST);
  return SUCCESS_EC;
}

void RunnerGpuUtils_OpenGL::clear_buffer(const glm::vec3& color, GLbitfield mask) {
  glClearColor(color.r, color.g, color.b, 1.0);
  glClear(mask);
}

}  // namespace uinta
