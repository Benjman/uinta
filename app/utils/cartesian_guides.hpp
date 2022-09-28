#ifndef UINTA_UTILS_CARTESIAN_GUIDES_HPP
#define UINTA_UTILS_CARTESIAN_GUIDES_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <uinta/buffer.hpp>
#include <uinta/gl/api.hpp>
#include <uinta/io/file_manager.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/shader.hpp>

namespace uinta {
/**
 * Utility to draw lines along the positive axis of each plane at a specified point.
 */
class CartesianGuides {
 public:
  void init(FileManager& fileManager, const Display& display);

  void doRender(const RunnerState& state, const glm::mat4& projView);

 private:
  Vao vao{{VertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0)}};
  Vbo vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint shader;
  GLuint u_mvp;
  GLuint u_color;
  GLuint u_viewport;
  GLuint u_bezierStrength;
};
}  // namespace uinta

#endif  // UINTA_UTILS_CARTESIAN_GUIDES_HPP
