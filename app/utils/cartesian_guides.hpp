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
 public:
  void init(FileManager& fileManager, const Display& display);

  void doRender(const RunnerState& state, const glm::mat4& projView);

 private:
  Vao vao{{{0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0}}};
  Vbo vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint shader;
  GLuint u_mvp;
  GLuint u_color;
  GLuint u_viewport;
  GLuint u_bezierStrength;
};

}  // namespace uinta

#endif  // UINTA_UTILS_CARTESIAN_GUIDES_HPP
