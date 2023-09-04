#ifndef UINTA_UTILS_CARTESIAN_GRID_HPP
#define UINTA_UTILS_CARTESIAN_GRID_HPP

#include <spdlog/fwd.h>

#include <glm/fwd.hpp>
#include <memory>
#include <uinta/error.hpp>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>

namespace uinta {

class FileManager;

class CartesianGridRenderer {
  friend class CartesianGrid;

 public:
  virtual uinta_error_code init(FileManager& fileManager) = 0;
  virtual void render(const glm::mat4& projectViewMatrix) const = 0;

 protected:
  Vao m_vao{{
      {0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 2 * sizeof(GLfloat)},
  }};
  Vbo m_vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint m_u_mvp = GL_ZERO;
  u32 m_vertex_count = 0;
  GLuint m_shader = GL_ZERO;
  f32 m_line_width = 1;
};

// TODO: Move me to the Uinta OpenGL library when it's made.
class CartesianGridRenderer_OpenGL : public CartesianGridRenderer {
 public:
  uinta_error_code init(FileManager& fileManager) override;
  void render(const glm::mat4& projectViewMatrix) const override;
};

class CartesianGrid {
  friend void settingsGrid(Runner&);

 public:
  CartesianGrid(std::unique_ptr<CartesianGridRenderer> renderer = nullptr);

  uinta_error_code init(FileManager& fileManager);
  void render(const glm::mat4& projView);

  const CartesianGridRenderer& renderer() const noexcept {
    assert(m_renderer && "Renderer must be initialized!");
    return *m_renderer;
  }

 private:
  std::unique_ptr<CartesianGridRenderer> m_renderer;
};

}  // namespace uinta

#endif  // UINTA_UTILS_CARTESIAN_GRID_HPP
