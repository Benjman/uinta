#ifndef UINTA_UTILS_CARTESIAN_GRID_HPP
#define UINTA_UTILS_CARTESIAN_GRID_HPP

#include <glm/fwd.hpp>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>

namespace uinta {

class CartesianGrid {
  friend bool initShader(CartesianGrid&, FileManager&);
  friend bool initGrid(CartesianGrid&);

 public:
  bool init(FileManager& fileManager);
  void render(const glm::mat4& projView);

 private:
  Vao vao{{
      {0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 2 * sizeof(GLfloat)},
  }};
  Vbo vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint shader;
  GLuint u_mvp;
  int vcount = 0;
};

}  // namespace uinta

#endif  // UINTA_UTILS_CARTESIAN_GRID_HPP
