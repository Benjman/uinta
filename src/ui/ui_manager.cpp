#include <spdlog/sinks/stdout_color_sinks.h>
#include <uinta/gl/api.h>

#include <glm/gtc/type_ptr.hpp>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene.hpp>
#include <uinta/shader.hpp>
#include <uinta/ui/ui_element.hpp>
#include <uinta/ui/ui_manager.hpp>

namespace uinta {

class UiRenderer_OpenGL : public UiRenderer {
 public:
  UiRenderer_OpenGL(const UiManager& ui) : UiRenderer(ui) {
  }

  uinta_error_code init(FileManager& fileManager, spdlog::logger* logger) noexcept override {
    if (!m_logger) m_logger = spdlog::stdout_color_mt(logger->name() + ":Renderer");

    m_resolution_dirty = true;

    const auto vs = fileManager.registerFile("shader/ui.vs");
    const auto fs = fileManager.registerFile("shader/ui.fs");
    fileManager.loadFile({vs, fs});

    const std::vector<std::string> sources = {fileManager.getDataString(vs), fileManager.getDataString(fs)};
    const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    if (auto error = createShaderProgram(m_shader, sources, stages, {"u_transform", "u_alpha", "u_projection"},
                                         {&m_u_transform, &m_u_alpha, &m_u_projection});
        error)
      return error;

    fileManager.releaseFile({vs, fs});

    m_vao.init(logger);
    m_vbo.init(logger);

    return SUCCESS_EC;
  }

  void start() noexcept override {
    glUseProgram(m_shader);
  }

  RenderState render() noexcept override {
    m_info.vertex_count = 0;
    m_vao.bind();
    m_vbo.bind();
    glUseProgram(m_shader);
    if (m_resolution_dirty) {
      glUniformMatrix4fv(m_u_projection, 1, GL_FALSE,
                         glm::value_ptr(glm::ortho<f32>(0, m_resolution.x, m_resolution.y, 0, -1, 1)));
      m_resolution_dirty = false;
      SPDLOG_LOGGER_DEBUG(m_logger, "Updated UI resolution: ({}, {})", m_resolution.x, m_resolution.y);
    }
    for (const auto& element : m_ui.elements()) {
      glDrawElements(GL_TRIANGLES, element->index_count(), GL_UNSIGNED_INT, 0);
      m_info.vertex_count += element->index_count();
    }
    return m_info;
  }

  virtual void upload(const std::span<f32> buffer, size_t offset) noexcept override {
    m_vao.bind();
    m_vbo.bind();
    m_vbo.upload(buffer, offset);
  }

  virtual void idx_buffer(const std::span<u32> buffer, size_t offset) noexcept override {
    m_vao.bind();
    m_vao.index_buffer(buffer);
  }

 private:
  Vao m_vao{{
      {0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat)},
  }};
  Vbo m_vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  u32 m_shader;
  u32 m_u_transform;
  u32 m_u_projection;
  u32 m_u_alpha;
};

UiManager::UiManager(Scene& scene, std::unique_ptr<UiRenderer> renderer) noexcept
    : m_logger(spdlog::stdout_color_mt(scene.logger()->name() + ":UI")),
      m_renderer(renderer ? std::move(renderer) : std::make_unique<UiRenderer_OpenGL>(*this)),
      m_scene(scene) {
  m_renderer->resolution({scene.runner().window().width, scene.runner().window().height});
  scene.runner().subscribe(RunnerEvents::WindowSizeChanged, [&](const Event* event) {
    if (const auto* e = static_cast<const WindowEvent*>(event); e) m_renderer->resolution({e->window.width, e->window.height});
  });
}

UiManager::~UiManager() {
  if (m_logger) spdlog::drop(m_logger->name());
}

uinta_error_code UiManager::init() {
  if (auto error = m_renderer->init(m_scene.runner().file_manager(), m_logger.get()); error) return error;
  // m_elements.emplace_back(std::make_unique<UiPane>(glm::vec3(15, 15, 0), glm::vec3(100, 30, 0), Anchor()));
  std::array<f32, KILOBYTES(20)> vtx_buffer;  // TODO: Find a smarter way to reserve adequate space
  std::array<u32, KILOBYTES(20)> idx_buffer;
  std::span<f32> vtx_span(vtx_buffer);
  std::span<u32> idx_span(idx_buffer);
  size_t vtx_offset = 0;
  size_t idx_offset = 0;
  for (const auto& element : m_elements) {
    if (auto error = element->init(); error) return error;
    element->generate_mesh(vtx_span.subspan(vtx_offset), idx_span.subspan(idx_offset));
    std::for_each(vtx_span.subspan(vtx_offset).begin(), vtx_span.subspan(vtx_offset).begin() + element->index_count(),
                  [vtx_offset](u32 idx) { return idx + vtx_offset; });
    vtx_offset += element->vertex_count() * UiElement::ElementsPerVertex;
    idx_offset += element->index_count();
  }
  m_renderer->upload(vtx_span.subspan(0, vtx_offset), 0);
  m_renderer->idx_buffer(idx_span.subspan(0, idx_offset), 0);
  return SUCCESS_EC;
}

void UiManager::render(const RunnerState& state) {
  m_renderer->render();
}

void UiManager::tick(const RunnerState& state, const InputState& input) {
  for (const auto& element : m_elements) element->tick(state.delta);
}

void UiManager::add_element(std::unique_ptr<UiElement> element) noexcept {
  m_elements.emplace_back(std::move(element));
}

std::span<const std::unique_ptr<UiElement>> UiManager::elements() const noexcept {
  return m_elements;
}

}  // namespace uinta
