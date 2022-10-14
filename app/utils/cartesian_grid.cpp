#include "./cartesian_grid.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <uinta/io/file_manager.hpp>
#include <uinta/mesh.hpp>
#include <uinta/shader.hpp>

namespace uinta {

void CartesianGrid::init(FileManager& fm) {
  initShader(*this, fm);
  initGrid(*this);
}

void initShader(CartesianGrid& grid, FileManager& fm) {
  auto vs = fm.registerFile("shader/cartesianGrid.vs");
  auto fs = fm.registerFile("shader/cartesianGrid.fs");
  fm.loadFile({vs, fs});

  std::vector<std::string> sources = {fm.getDataString(vs), fm.getDataString(fs)};
  std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  grid.shader = createShaderProgram(sources, stages, {"u_mvp"}, {&grid.u_mvp});
  fm.releaseFile({vs, fs});
}

void initGrid(CartesianGrid& grid) {
  float buffer[600];
  float lineSize = 0.005;

  auto colorX = glm::vec3(155, 34, 38) / 255.0f;
  auto colorZ = glm::vec3(0, 95, 115) / 255.0f;
  auto colorPrimary = glm::vec3(0, 25, 32) / 255.0f;

  // TODO draw origins last so the primary lines aren't overlapping
  for (int isVert = 0; isVert <= 1; isVert++) {
    for (int i = -4; i <= 5; i++) {
      // clang-format off
      auto v0 = isVert ? glm::vec2(-lineSize + i,  5) : glm::vec2(-5,  lineSize + i);
      auto v1 = isVert ? glm::vec2(-lineSize + i, -5) : glm::vec2(-5, -lineSize + i);
      auto v2 = isVert ? glm::vec2( lineSize + i, -5) : glm::vec2( 5, -lineSize + i);
      auto v3 = isVert ? glm::vec2( lineSize + i,  5) : glm::vec2( 5,  lineSize + i);

      auto color = colorPrimary;
      if (i == 0) color = isVert ? colorZ : colorX;

      float vertices[] = {
        v0.x, v0.y, color.r, color.g, color.b,
        v1.x, v1.y, color.r, color.g, color.b,
        v2.x, v2.y, color.r, color.g, color.b,
        v2.x, v2.y, color.r, color.g, color.b,
        v3.x, v3.y, color.r, color.g, color.b,
        v0.x, v0.y, color.r, color.g, color.b,
      };
      // clang-format on

      memcpy(&buffer[grid.vcount * 5], vertices, sizeof(vertices));
      grid.vcount += 6;
    }
  }

  initVao(grid.vao);
  upload(grid.vbo, buffer, grid.vcount * 6 * sizeof(float));
  initVertexAttribs(grid.vao);
}

void CartesianGrid::render(const glm::mat4& projView) {
  glUseProgram(shader);
  bind(vao);

  // TODO billboard quads http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards
  // TODO draw range based on view frustum https://stackoverflow.com/questions/12836967
  for (int z = -1; z <= 1; z++) {
    for (int x = -1; x <= 1; x++) {
      glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * glm::translate(glm::mat4(1), {x * 10, 0, z * 10})));
      glDrawArrays(GL_TRIANGLES, 0, vcount);
    }
  }
}

}  // namespace uinta
