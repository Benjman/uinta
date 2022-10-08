#ifndef UINTA_UTILS_CARTESIAN_GUIDES_HPP
#define UINTA_UTILS_CARTESIAN_GUIDES_HPP

#include <glm/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>

namespace uinta {

class FileManager;
struct Display;
struct RunnerState;

/**
 * Utility to draw lines along the positive axis of each plane
 */
class CartesianGuides {
  friend void initShader(CartesianGuides&, FileManager&);
  friend void initGrid(CartesianGuides&);

 public:
  void init(FileManager& fileManager);
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

#endif  // UINTA_UTILS_CARTESIAN_GUIDES_HPP
