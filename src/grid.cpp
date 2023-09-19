#include <spdlog/sinks/stdout_color_sinks.h>

#include <glm/gtc/type_ptr.hpp>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/mesh.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/shader.hpp>
#include <uinta/grid.hpp>

namespace uinta {

enum class error {
  InitShader = 100,
  InitMesh = 200,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::InitShader), "Failed to initialize shader!"},
    {static_cast<uinta_error_code_t>(error::InitMesh), "Failed to initialize mesh!"},
};

UINTA_ERROR_FRAMEWORK(Grid, errorMessages);

Grid::Grid(const Scene& scene, std::unique_ptr<GridRenderer> renderer)
    : m_renderer(renderer ? std::move(renderer) : std::make_unique<GridRenderer_OpenGL>(*this)),
      m_logger(spdlog::stdout_color_mt(scene.runner().logger()->name() + ":Grid")) {
  assert(m_renderer && "Renderer must be initialized!");
}

uinta_error_code Grid::init(FileManager& fm) {
  SPDLOG_LOGGER_INFO(m_logger, "Initializing grid...");
  if (auto error = m_renderer->init(fm); error) return error;

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

      memcpy(&buffer[m_renderer->vertex_count() * 5], vertices, sizeof(vertices));
      m_renderer->vertex_count(m_renderer->vertex_count() + 2);
    }
  }

  m_renderer->upload(buffer, m_renderer->vertex_count() * 5 * sizeof(f32), 0);

  SPDLOG_LOGGER_INFO(m_logger, "Initialized grid.");
  return SUCCESS_EC;
}

void Grid::render(const glm::mat4& projView) {
  m_renderer->render(projView);
}

GridRenderer::GridRenderer(const Grid& grid) : m_logger(grid.logger()) {
}

uinta_error_code GridRenderer_OpenGL::init(FileManager& fileManager) {
  const auto vs = fileManager.registerFile("shader/grid.vs");
  const auto fs = fileManager.registerFile("shader/grid.fs");
  fileManager.loadFile({vs, fs});

  const std::vector<std::string> sources = {fileManager.getDataString(vs), fileManager.getDataString(fs)};
  const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  if (auto error = createShaderProgram(m_shader, sources, stages, {"u_mvp"}, {&m_u_mvp}); error) return error;
  fileManager.releaseFile({vs, fs});

  if (m_shader == GL_ZERO) return make_error(error::InitMesh);

  m_vao.init(m_logger);
  m_vbo.init(m_logger);
  return SUCCESS_EC;
}

void GridRenderer_OpenGL::render(const glm::mat4& projectViewMatrix) const {
  m_vao.bind();
  m_vbo.bind();
  static constexpr i32 GRID_RADIUS = 5;
  glUseProgram(m_shader);
  i32 currentLineWidth;
  glGetIntegerv(GL_LINE_WIDTH, &currentLineWidth);
  if (m_line_width != currentLineWidth) glLineWidth(m_line_width);
  for (i32 z = -GRID_RADIUS; z <= GRID_RADIUS; z++) {
    for (i32 x = -GRID_RADIUS; x <= GRID_RADIUS; x++) {
      glUniformMatrix4fv(m_u_mvp, 1, GL_FALSE,
                         glm::value_ptr(projectViewMatrix * glm::translate(glm::mat4(1), {x * 10, 0, z * 10})));
      glDrawArrays(GL_LINES, 0, m_vertex_count);
    }
  }
  if (m_line_width != currentLineWidth) glLineWidth(currentLineWidth);
}

void GridRenderer_OpenGL::upload(const f32* const buffer, size_t size, size_t offset) {
  m_vao.bind();
  m_vbo.bind();
  m_vbo.upload(buffer, size, offset);
  m_vao.init_attributes();
}

}  // namespace uinta
