#ifndef UINTA_GRID_HPP
#define UINTA_GRID_HPP

#include <spdlog/fwd.h>

#include <glm/fwd.hpp>
#include <memory>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/scene.hpp>

namespace uinta {

class GridRenderer {
 public:
  GridRenderer() = default;

  virtual uinta_error_code init(FileManager& fileManager, spdlog::logger* logger) = 0;
  virtual void render(const glm::mat4& projectViewMatrix) const = 0;
  virtual void upload(const f32* const buffer, size_t size, size_t offset) = 0;

  u32 vertex_count() const noexcept {
    return m_vertex_count;
  }

  void vertex_count(u32 count) noexcept {
    m_vertex_count = count;
  }

 protected:
  u32 m_u_mvp = 0;
  u32 m_vertex_count = 0;
  u32 m_shader = 0;
  f32 m_line_width = 1;
};

class GridScene : public Scene {
 public:
  GridScene(Runner& runner, std::unique_ptr<GridRenderer> renderer = nullptr);

  uinta_error_code init() override;

  void render(const RunnerState& state) override;

 private:
  std::unique_ptr<GridRenderer> m_renderer;
  const TargetCamera* m_camera;
};

}  // namespace uinta

#endif  // UINTA_GRID_HPP
