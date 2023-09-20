#ifndef UINTA_RUNNER_DEPENDENCIES_HPP
#define UINTA_RUNNER_DEPENDENCIES_HPP

#include <uinta/gl/api.h>

#include <glm/fwd.hpp>
#include <uinta/file_manager.hpp>

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

}  // namespace uinta

#endif  // UINTA_RUNNER_DEPENDENCIES_HPP
