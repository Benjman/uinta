#ifndef UINTA_RUNNER_DEPENDENCIES_HPP
#define UINTA_RUNNER_DEPENDENCIES_HPP

#include <glm/fwd.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/fwd.hpp>

namespace uinta {

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

}  // namespace uinta

#endif  // UINTA_RUNNER_DEPENDENCIES_HPP
