#ifndef UINTA_POST_PROCESSING_RENDER_TARGET_HPP
#define UINTA_POST_PROCESSING_RENDER_TARGET_HPP

#include <glm/mat4x4.hpp>
#include <uinta/buffer.hpp>

namespace uinta {

struct RenderTarget {
  glm::mat4 model = glm::mat4(1.0);
  GpuMemoryContext gpu_mem;

  RenderTarget() {
  }

  RenderTarget(GpuMemoryContext& gpu_mem) : gpu_mem(gpu_mem) {
  }

  RenderTarget(const RenderTarget& other) {
    *this = other;
  }

  RenderTarget& operator=(const RenderTarget& other) {
    model = glm::mat4(other.model);
    gpu_mem = other.gpu_mem;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_POST_PROCESSING_RENDER_TARGET_HPP
