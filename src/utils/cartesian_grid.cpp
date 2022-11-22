#include <glm/gtc/type_ptr.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/mesh.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

bool CartesianGrid::init(FileManager& fm) {
  SPDLOG_INFO("Initializing grid...");
  if (!initShader(*this, fm)) {
    SPDLOG_ERROR("Failed to initialize CartesianGrid shader!");
    return false;
  }
  if (!initGrid(*this)) {
    SPDLOG_ERROR("Failed to initialize CartesianGrid mesh!");
    return false;
  }
  SPDLOG_INFO("Initialized grid.");
  return true;
}

bool initShader(CartesianGrid& grid, FileManager& fm) {
  auto vs = fm.registerFile("shader/cartesianGrid.vs");
  auto fs = fm.registerFile("shader/cartesianGrid.fs");
  fm.loadFile({vs, fs});

  std::vector<std::string> sources = {fm.getDataString(vs), fm.getDataString(fs)};
  std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  grid.shader = createShaderProgram(sources, stages, {"u_mvp"}, {&grid.u_mvp});
  fm.releaseFile({vs, fs});
  return grid.shader != GL_ZERO;
}

bool initGrid(CartesianGrid& grid) {
  float buffer[660];
  float lineSize = 0.005;

  auto colorX = glm::vec3(155, 34, 38) / 255.0f;
  auto colorZ = glm::vec3(0, 95, 115) / 255.0f;
  auto colorPrimary = glm::vec3(0, 25, 32) / 255.0f;

  // TODO draw origins last so the primary lines aren't overlapping
  for (int isVert = 0; isVert <= 1; isVert++) {
    for (int i = -5; i <= 5; i++) {
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

  return grid.vao.id != GL_ZERO || grid.vbo.id != GL_ZERO;
}

void CartesianGrid::render(const glm::mat4& projView) {
  glEnable(GL_DEPTH_TEST);
  glUseProgram(shader);
  bind(vao);

  // TODO billboard quads http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards
  // TODO draw range based on view frustum https://stackoverflow.com/questions/12836967
  for (int z = -5; z <= 5; z++) {
    for (int x = -5; x <= 5; x++) {
      glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * glm::translate(glm::mat4(1), {x * 10, 0, z * 10})));
      glDrawArrays(GL_TRIANGLES, 0, vcount);
    }
  }
}

}  // namespace uinta
