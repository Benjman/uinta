#include <uinta/gl/api.h>
#include <uinta/glfw/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <uinta/exception.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/font.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene.hpp>
#include <uinta/shader.hpp>
#include <uinta/text.hpp>
#include <uinta/ui/ui_manager.hpp>

#include "./font_atlas_scene.cpp"

namespace uinta {

class UiScene : public Scene {
 public:
  UiScene(Runner& runner) : Scene("UI", runner, Scene::Layer::UI), m_ui(*this) {
    auto window = runner.window();
    window.width = 1440;
    window.height = 900;
    runner.window(window);
  }

  ~UiScene() override = default;

  uinta_error_code init() override {
    if (auto error = m_font.load(runner().file_manager(), logger()); error) return error;
    if (auto error = m_ui.init(); error) return error;
    if (auto error = runner().add_scene(std::make_unique<FontAtlasScene>(runner(), m_font)); error) return error;
    m_atlas_scene = runner().find_scene<FontAtlasScene>();

    std::vector<f32> vertex_buffer(m_mesh.vertex_buffer_size());
    std::vector<u32> index_buffer(m_mesh.index_buffer_size());
    if (auto error = m_mesh.generate(m_font, vertex_buffer, index_buffer, logger()); error) return error;

    m_vao.init(logger());
    m_vbo.init(logger());
    m_vbo.upload(vertex_buffer);
    m_vao.index_buffer(index_buffer);
    m_vao.init_attributes();

    return transition(State::Running);
  }

  void pre_tick(const RunnerState& state, const InputState& input) override {
    Scene::pre_tick(state, input);
    m_ui.pre_tick(state, input);
  }

  void tick(const RunnerState& state, const InputState& input) override {
    Scene::tick(state, input);
    m_ui.tick(state, input);
  }

  void post_tick(const RunnerState& state, const InputState& input) override {
    Scene::post_tick(state, input);
    m_ui.post_tick(state, input);
  }

  void pre_render(const RunnerState& state) override {
    Scene::pre_render(state);
    m_ui.pre_render(state);
  }

  void render(const RunnerState& state) override {
    Scene::render(state);
    m_ui.render(state);
    m_atlas_scene->start();
    m_vao.bind();
    m_vbo.bind();
    glDrawElements(GL_TRIANGLES, m_mesh.index_buffer_size(), GL_UNSIGNED_INT, 0);
    m_atlas_scene->stop();
  }

  void post_render(const RunnerState& state) override {
    Scene::post_render(state);
    m_ui.post_render(state);
    static bool ShowAtlas = false;
    if (ImGui::CollapsingHeader("Font") && ImGui::Checkbox("Show Atlas", &ShowAtlas)) {
      if (ShowAtlas) {
        if (auto error = runner().find_scene<FontAtlasScene>()->transition(State::Running); error) throw UintaException(error);
      } else {
        if (auto error = runner().find_scene<FontAtlasScene>()->transition(State::Paused); error) throw UintaException(error);
      }
    }
  }

  void shutdown() override {
    Scene::shutdown();
    m_ui.shutdown();
  }

 private:
  UiManager m_ui;
  Font m_font = Font("font/dejavu/DejaVuSans.ttf");
  Vao m_vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)},
      {2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 5 * sizeof(GLfloat)},
  }};
  Vbo m_vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  TextMesh m_mesh = TextMesh("Hello text rendering!", 32);
  FontAtlasScene* m_atlas_scene;
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  using namespace uinta;
  auto runner = GlfwRunner("UI App", argc, argv);
  if (auto error = runner.add_scene(std::make_unique<UiScene>(runner)); error) throw UintaException(error);
  return runner.run();
}
