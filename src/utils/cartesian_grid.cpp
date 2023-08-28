#include <glm/gtc/type_ptr.hpp>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/mesh.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

enum class error {
  InitShader = 100,
  InitMesh = 200,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::InitShader), "Failed to initialize shader!"},
    {static_cast<uinta_error_code_t>(error::InitMesh), "Failed to initialize mesh!"},
};

UINTA_ERROR_FRAMEWORK(CartesianGrid, errorMessages);

uinta_error_code CartesianGrid::init(FileManager& fm) {
  SPDLOG_INFO("Initializing grid...");
  if (auto error = initShader(*this, fm); error) return error;
  if (auto error = initGrid(*this); error) return error;
  SPDLOG_INFO("Initialized grid.");
  return SUCCESS_EC;
}

uinta_error_code initShader(CartesianGrid& grid, FileManager& fm) {
  const auto vs = fm.registerFile("shader/cartesianGrid.vs");
  const auto fs = fm.registerFile("shader/cartesianGrid.fs");
  fm.loadFile({vs, fs});

  const std::vector<std::string> sources = {fm.getDataString(vs), fm.getDataString(fs)};
  const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  if (auto error = createShaderProgram(grid.shader, sources, stages, {"u_mvp"}, {&grid.u_mvp}); error) return error;
  fm.releaseFile({vs, fs});

  if (grid.shader == GL_ZERO) return make_error(error::InitMesh);
  return SUCCESS_EC;
}

uinta_error_code initGrid(CartesianGrid& grid) {
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

  if (auto error = initVao(grid.vao); error) return error;
  if (auto error = uploadVbo(grid.vbo, buffer, grid.vcount * 5 * sizeof(f32)); error) return error;
  if (auto error = initVertexAttribs(grid.vao); error) return error;

  if (grid.vao.id == GL_ZERO || grid.vbo.id == GL_ZERO) return make_error(error::InitMesh);
  return SUCCESS_EC;
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
