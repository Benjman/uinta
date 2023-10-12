// clang-format off
#include <chrono>
#include <spdlog/stopwatch.h>
// clang-format on

#include <spdlog/sinks/stdout_color_sinks.h>
#include <uinta/gl/api.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene.hpp>

namespace uinta {

enum class error : u8 {
  SceneInvalid = 10,
  SceneNotFound = 10,
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
      m_gpu_utils(std::move(gpu_utils)),
      m_clear_mask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) {
  assert(m_file_manager && "File manager must be initialized!");
  assert(m_gpu_utils && "GPU Utilities must be initialized!");
  processArgs(this, argc, argv);
  initSpdlog();
  if (auto error = m_file_manager->init(*this); error) throw UintaException(error);
  SPDLOG_LOGGER_INFO(m_logger, "Runner started for '{}'.", title);
}

Runner::~Runner() noexcept {
  if (m_logger) spdlog::drop(m_logger->name());
}

i32 Runner::run() noexcept {
  spdlog::stopwatch sw;
  if (auto error = m_gpu_utils->init(*this); error) {
    SPDLOG_LOGGER_CRITICAL(m_logger, "Error attempting to initialize GPU utilites: ", error.message());
    return error.value();
  }
  publish_queued_events();
  SPDLOG_LOGGER_INFO(m_logger, "Initialized in {} seconds.", sw.elapsed().count());
  setFlag(IS_RUNNING, true, m_flags);
  while (isFlagSet(IS_RUNNING, m_flags)) {
    reset(m_input);
    pollInput();
    advanceState();
    for (auto& scene : m_scenes) {
      if (Scene::State::Created == scene->state()) {
        if (auto error = scene->init(); error) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "Failed to initialize: {}", error.message());
          (void)scene->transition(Scene::State::Destroyed);
        }
      }
    }
    publish_queued_events();
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->pre_tick(m_state, m_input);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "pre_tick exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->tick(m_state, m_input);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "tick exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->post_tick(m_state, m_input);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "post_tick exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    for (auto itr = m_scenes.cbegin(); itr != m_scenes.cend();) {
      if (Scene::State::Destroyed == itr->get()->state()) {
        itr = m_scenes.erase(itr);
      } else
        itr++;
    }
    publish_queued_events();
    m_gpu_utils->clear_buffer(m_clear_color, m_clear_mask);
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->pre_render(m_state);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "pre_render exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->render(m_state);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "render exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    for (auto& scene : m_scenes) {
      if (Scene::State::Running == scene->state()) {
        try {
          scene->post_render(m_state);
        } catch (const UintaException& ex) {
          SPDLOG_LOGGER_ERROR(scene->logger(), "post_render exception: {}", ex.what());
        }
      }
    }
    publish_queued_events();
    swapBuffers();
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
  auto win = window();
  win.width = width;
  win.height = height;
  m_window = win;
  publish(RunnerEvents::WindowSizeChanged, WindowEvent(m_state.runtime, m_window));
  SPDLOG_LOGGER_DEBUG(m_logger, "Window size updated: {}x{}.", width, height);
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

void Runner::add_event(event_t event_type, std::unique_ptr<const Event> event) noexcept {
  queue_event(event_type, std::move(event));
}

}  // namespace uinta
