#ifndef UINTA_RENDERER_HPP
#define UINTA_RENDERER_HPP

#include <spdlog/fwd.h>

#include <glm/fwd.hpp>
#include <uinta/fwd.hpp>

namespace uinta {

struct RenderState {
  RenderState(u32 vertex_count = 0) : vertex_count(vertex_count) {
  }

  u32 vertex_count;
};

class Renderer {
 public:
  virtual uinta_error_code init(FileManager& fileManager, spdlog::logger* logger) noexcept = 0;

  virtual RenderState render() noexcept = 0;

  virtual void start() noexcept {
    m_info.vertex_count = 0;
  }

  const RenderState& info() const noexcept {
    return m_info;
  }

  void vertex_count(u32 vertex_count) noexcept {
    m_info.vertex_count = vertex_count;
  }

 protected:
  RenderState m_info;
};

}  // namespace uinta

#endif  // UINTA_RENDERER_HPP
