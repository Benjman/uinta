#ifndef UINTA_UI_RENDERER_HPP
#define UINTA_UI_RENDERER_HPP

#include <glm/vec2.hpp>
#include <memory>
#include <span>
#include <uinta/fwd.hpp>
#include <uinta/renderer.hpp>

namespace uinta {

class UiRenderer : public Renderer {
 public:
  UiRenderer(const UiManager& ui) : m_ui(ui) {
  }

  ~UiRenderer();

  virtual void start() noexcept override {
    Renderer::start();
  }

  RenderState render() noexcept override {
    return {0};
  }

  virtual void upload(const std::span<f32> buffer, size_t offset) noexcept = 0;

  virtual void idx_buffer(const std::span<u32> buffer, size_t offset) noexcept = 0;

  void resolution(const glm::vec2& resolution) noexcept {
    m_resolution = resolution;
    m_resolution_dirty = true;
  }

 protected:
  const UiManager& m_ui;
  glm::vec2 m_resolution;
  bool m_resolution_dirty;
  std::shared_ptr<spdlog::logger> m_logger;
};

}  // namespace uinta

#endif  // UINTA_UI_RENDERER_HPP
