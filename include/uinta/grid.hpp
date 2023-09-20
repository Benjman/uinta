#ifndef UINTA_GRID_HPP
#define UINTA_GRID_HPP

#include <spdlog/fwd.h>

#include <glm/fwd.hpp>
#include <memory>
#include <uinta/error.hpp>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>

namespace uinta {

class GridRenderer {
 public:
  GridRenderer(const Grid& grid);

  virtual uinta_error_code init(FileManager& fileManager) = 0;
  virtual void render(const glm::mat4& projectViewMatrix) const = 0;
  virtual void upload(const f32* const buffer, size_t size, size_t offset) = 0;

  u32 vertex_count() const noexcept {
    return m_vertex_count;
  }

  void vertex_count(u32 count) noexcept {
    m_vertex_count = count;
  }

 protected:
  GLuint m_u_mvp = GL_ZERO;
  u32 m_vertex_count = 0;
  GLuint m_shader = GL_ZERO;
  f32 m_line_width = 1;
  const spdlog::logger* m_logger;
};

class GridRenderer_OpenGL : public GridRenderer {
 public:
  GridRenderer_OpenGL(const Grid& grid) : GridRenderer(grid) {
  }

  uinta_error_code init(FileManager& fileManager) override;
  void render(const glm::mat4& projectViewMatrix) const override;
  void upload(const f32* const buffer, size_t size, size_t offset) override;

 private:
  Vao m_vao{{
      {0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 2 * sizeof(GLfloat)},
  }};
  Vbo m_vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
};

class Grid {
 public:
  Grid(const Scene& scene, std::unique_ptr<GridRenderer> renderer = nullptr);

  uinta_error_code init(FileManager& fileManager);

  void render(const glm::mat4& projView);

  const spdlog::logger* logger() const noexcept {
    return m_logger.get();
  }

  const GridRenderer& renderer() const noexcept {
    return *m_renderer;
  }

 private:
  std::unique_ptr<GridRenderer> m_renderer;
  std::shared_ptr<spdlog::logger> m_logger;
};

}  // namespace uinta

#endif  // UINTA_GRID_HPP
