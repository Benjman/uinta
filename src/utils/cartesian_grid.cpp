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
  const auto vs = fm.registerFile("shader/cartesianGrid.vs");
  const auto fs = fm.registerFile("shader/cartesianGrid.fs");
  fm.loadFile({vs, fs});

  const std::vector<std::string> sources = {fm.getDataString(vs), fm.getDataString(fs)};
  const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  grid.shader = createShaderProgram(sources, stages, {"u_mvp"}, {&grid.u_mvp});
  fm.releaseFile({vs, fs});
  return grid.shader != GL_ZERO;
}

bool initGrid(CartesianGrid& grid) {
  f32 buffer[220];

  constexpr auto colorX = glm::vec3(155, 34, 38) / 255.0f;
  constexpr auto colorZ = glm::vec3(0, 95, 115) / 255.0f;
  constexpr auto colorPrimary = glm::vec3(0, 25, 32) / 255.0f;

  for (i32 isVert = 0; isVert <= 1; isVert++) {
    for (i32 i = -5; i <= 5; i++) {
      // clang-format off
      const auto v0 = isVert ? glm::vec2(i,  5) : glm::vec2( 5, i);
      const auto v1 = isVert ? glm::vec2(i, -5) : glm::vec2(-5, i);

      auto color = colorPrimary;
      if (i == 0) color = isVert ? colorZ : colorX;

      const f32 vertices[] = {
        v0.x, v0.y, color.r, color.g, color.b,
        v1.x, v1.y, color.r, color.g, color.b,
      };
      // clang-format on

      memcpy(&buffer[grid.vcount * 5], vertices, sizeof(vertices));
      grid.vcount += 2;
    }
  }

  initVao(grid.vao);
  uploadVbo(grid.vbo, buffer, grid.vcount * 5 * sizeof(f32));
  initVertexAttribs(grid.vao);

  return grid.vao.id != GL_ZERO || grid.vbo.id != GL_ZERO;
}

void CartesianGrid::render(const glm::mat4& projView) {
  glUseProgram(shader);
  bindVao(vao);
  i32 currentLineWidth;
  glGetIntegerv(GL_LINE_WIDTH, &currentLineWidth);
  if (lineWidth != currentLineWidth) glLineWidth(lineWidth);
  for (i32 z = -5; z <= 5; z++) {
    for (i32 x = -5; x <= 5; x++) {
      glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * glm::translate(glm::mat4(1), {x * 10, 0, z * 10})));
      glDrawArrays(GL_LINES, 0, vcount);
    }
  }
  if (lineWidth != currentLineWidth) glLineWidth(currentLineWidth);
}

}  // namespace uinta
