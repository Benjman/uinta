#include <uinta/gl/api.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <span>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/font.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene.hpp>
#include <uinta/shader.hpp>

namespace uinta {

class FontAtlasScene : public Scene {
 public:
  FontAtlasScene(Runner& runner, const Font& font) : Scene("Font Atlas", runner, Scene::Layer::Debug), m_font(font) {
    m_resolution = {runner.window().width, runner.window().height};
    runner.subscribe(RunnerEvents::WindowSizeChanged, [&](const Event* event) {
      if (const auto* e = static_cast<const WindowEvent*>(event); e) m_resolution = {e->window.width, e->window.height};
    });
  }

  uinta_error_code init() override {
    if (auto error = init_shader(); error) return error;
    if (auto error = init_atlas_quad(); error) return error;
    if (auto error = upload_texture(); error) return error;
    return transition(State::Paused);
  }

  void start() noexcept {
    glUseProgram(m_shader);
    m_vao.bind();
    m_vbo.bind();
    if (m_resolution_dirty) {
      glUniformMatrix4fv(m_u_projection, 1, GL_FALSE,
                         glm::value_ptr(glm::ortho<f32>(0, m_resolution.x, m_resolution.y, 0, -1, 1)));
      m_resolution_dirty = false;
      SPDLOG_LOGGER_DEBUG(logger(), "Updated Font Renderer resolution: ({}, {})", m_resolution.x, m_resolution.y);
    }
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void stop() noexcept {
    glDisable(GL_BLEND);
  }

  void render(const RunnerState& state) override {
    start();
    constexpr u8 IndexCount = 6;
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
    stop();
  }

 private:
  Vao m_vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)},
      {2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 5 * sizeof(GLfloat)},
  }};
  Vbo m_vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  const Font& m_font;
  glm::vec2 m_resolution;
  u32 m_shader = 0;
  u32 m_texture_id;
  u32 m_u_atlas = 0;
  u32 m_u_projection = 0;
  bool m_resolution_dirty = true;

  uinta_error_code init_shader() noexcept {
    const auto vs = runner().file_manager().registerFile("shader/font.vs");
    const auto fs = runner().file_manager().registerFile("shader/font.fs");
    runner().file_manager().loadFile({vs, fs});
    const std::vector<std::string> sources = {runner().file_manager().getDataString(vs),
                                              runner().file_manager().getDataString(fs)};
    const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    if (auto error = createShaderProgram(m_shader, sources, stages, {"u_atlas", "u_projection"}, {&m_u_atlas, &m_u_projection});
        error)
      return error;
    runner().file_manager().releaseFile({vs, fs});
    return SUCCESS_EC;
  }

  uinta_error_code init_atlas_quad() noexcept {
    m_vao.init(logger());
    m_vbo.init(logger());
    glm::vec3 top_left(60, 60, 0);
    glm::vec3 bottom_right(800, 800, 0);
    glm::vec3 color(1);
    std::array<f32, 32> buffer = {
        top_left.x,     top_left.y,     top_left.z, 0, 0, color.r, color.g, color.b,
        top_left.x,     bottom_right.y, top_left.z, 0, 1, color.r, color.g, color.b,
        bottom_right.x, bottom_right.y, top_left.z, 1, 1, color.r, color.g, color.b,
        bottom_right.x, top_left.y,     top_left.z, 1, 0, color.r, color.g, color.b,
    };
    m_vbo.upload(buffer);
    std::array<u32, 6> idx_buffer = {
        0, 1, 2, 2, 3, 0,
    };
    m_vao.index_buffer().init(logger());
    m_vao.index_buffer().upload(idx_buffer);
    m_vao.init_attributes();
    return SUCCESS_EC;
  }

  uinta_error_code upload_texture() noexcept {
    // upload font:
    assert((m_font.bitmap().size() & (m_font.bitmap().size() - 1)) == 0 && "Bitmap size must be a power of 2.");
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sqrt(m_font.bitmap().size()), sqrt(m_font.bitmap().size()), 0, GL_RED,
                 GL_UNSIGNED_BYTE, m_font.bitmap().data());
    glUseProgram(m_shader);
    glUniform1i(m_u_atlas, 0);
    return SUCCESS_EC;
  }
};

}  // namespace uinta
