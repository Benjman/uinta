#ifndef UINTA_RUNNER_DEPENDENCIES_HPP
#define UINTA_RUNNER_DEPENDENCIES_HPP

#include <uinta/gl/api.h>

#include <glm/fwd.hpp>
#include <memory>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/scene/renderer.hpp>

namespace uinta {

class RunnerGpuUtils {
 public:
  virtual uinta_error_code init() = 0;
  virtual void clear_buffer(const glm::vec3& color, GLbitfield mask) = 0;
};

class RunnerGpuUtils_OpenGL : public RunnerGpuUtils {
 public:
  uinta_error_code init() override;
  void clear_buffer(const glm::vec3& color, GLbitfield mask) override;
};

struct RunnerDependencies final {
  std::unique_ptr<FileManager> file_manager = std::make_unique<FileManager_Desktop>();
  std::unique_ptr<RunnerGpuUtils> gpu_utils = std::make_unique<RunnerGpuUtils_OpenGL>();
  std::unique_ptr<SceneRenderer> scene_renderer = std::make_unique<SceneRenderer_OpenGL>();

  RunnerDependencies(RunnerDependencies&& other) noexcept
      : file_manager(std::move(other.file_manager)),
        gpu_utils(std::move(other.gpu_utils)),
        scene_renderer(std::move(other.scene_renderer)) {
  }

  RunnerDependencies& operator=(RunnerDependencies&& other) noexcept {
    if (this == &other) return *this;
    file_manager = std::move(other.file_manager);
    gpu_utils = std::move(other.gpu_utils);
    scene_renderer = std::move(other.scene_renderer);
    return *this;
  }

  RunnerDependencies(const RunnerDependencies&) = delete;
  RunnerDependencies& operator=(const RunnerDependencies&) = delete;

  RunnerDependencies() = default;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_DEPENDENCIES_HPP
