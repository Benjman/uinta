#ifndef UINTA_GRID_HPP
#define UINTA_GRID_HPP

#include <spdlog/fwd.h>

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/renderer.hpp>
#include <uinta/scene.hpp>

namespace uinta {

class GridRenderer;

class GridScene : public Scene {
 public:
  GridScene(Runner& runner, std::unique_ptr<GridRenderer> renderer = nullptr);

  uinta_error_code init() override;

  void render(const RunnerState& state) override;

 private:
  std::unique_ptr<GridRenderer> m_renderer;
  const TargetCamera* m_camera;
};

class GridRenderer : public Renderer {
 public:
  GridRenderer() = default;

  virtual uinta_error_code init(FileManager& fileManager, spdlog::logger* logger) noexcept override = 0;

  virtual RenderState render() noexcept override = 0;

  virtual void upload(const f32* const buffer, size_t size, size_t offset) = 0;

  const glm::mat4& projection_view() const noexcept {
    return m_projection_view;
  }

  void projection_view(const glm::mat4& projection_view) noexcept {
    m_projection_view = projection_view;
  }

 protected:
  u32 m_u_mvp = 0;
  u32 m_shader = 0;
  f32 m_line_width = 1;
  glm::mat4 m_projection_view = glm::mat4(1);
};

}  // namespace uinta

#endif  // UINTA_GRID_HPP
