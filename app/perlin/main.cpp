#include <algorithm>
#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/math/perlin.hpp>
#include <uinta/scene.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/buffer2d.hpp>

namespace uinta {

class PerlinScene : public Scene {
 public:
  PerlinScene(Runner& runner) : Scene("Perlin", runner, Layer::Debug) {
  }

  ~PerlinScene() override = default;

  uinta_error_code init() override {
    const auto vs = runner().file_manager().registerFile("shader/texture.vs");
    const auto fs = runner().file_manager().registerFile("shader/texture.fs");
    runner().file_manager().loadFile({vs, fs});
    const std::vector<std::string> srcs({runner().file_manager().getDataString(vs), runner().file_manager().getDataString(fs)});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    if (auto error = createShaderProgram(shader_id, srcs, stages); error) return error;
    runner().file_manager().releaseFile({vs, fs});
    SPDLOG_INFO("Initialized shader {}.", shader_id);

    // clang-format off
    const f32 vertices[] = {
        // positions    // texture coords
         1,  1,         1, 1,
         1, -1,         1, 0,
        -1, -1,         0, 0,
        -1,  1,         0, 1,
    };
    const u32 indices[] = { 0, 1, 3, 1, 2, 3 };
    // clang-format on

    vao.init(logger());
    vbo.init(logger());
    vbo.upload(vertices, sizeof(vertices), 0);
    vao.index_buffer().upload(indices, sizeof(indices), 0);
    vao.init_attributes();

    glGenTextures(1, &texture_id);
    updateTexture();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SPDLOG_INFO("");
    SPDLOG_INFO("    ,========  KEYBOARD INPUTS  ========.");
    SPDLOG_INFO("    |     Action  |  Inc  |     Dec     |");
    SPDLOG_INFO("    |-----------------------------------|");
    SPDLOG_INFO("    |  Frequency  |   F   |  Shift + F  |");
    SPDLOG_INFO("    |   Offset-X  | Right |     Left    |");
    SPDLOG_INFO("    |   Offset-Y  |   Up  |     Down    |");
    SPDLOG_INFO("    |     Octave  |   O   |  Shift + O  |");
    SPDLOG_INFO("    |       Seed  |   S   |  Shift + S  |");
    SPDLOG_INFO("    `-----------------------------------'");
    SPDLOG_INFO("");

    return transition(State::Running);
  }

  void tick(const RunnerState& state, const InputState& input) override {
    static constexpr i32 OFFSET_DELTA = 10;
    if (isKeyPressed(input, KEY_S)) {
      perlin_seed += isShiftDown(input) ? -1 : 1;
      SPDLOG_INFO("Seed set to {}", perlin_seed);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_F)) {
      perlin_freq = std::clamp(perlin_freq + (isShiftDown(input) ? -1 : 1), 0.0f, 64.0f);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_O)) {
      perlin_oct = std::clamp(static_cast<i32>(perlin_oct + (isShiftDown(input) ? -1 : 1)), 0, 16);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_LEFT)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input) ? 10 : 1);
      offset += glm::ivec2(-delta, 0);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_RIGHT)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input) ? 10 : 1);
      offset += glm::ivec2(delta, 0);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_UP)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input) ? 10 : 1);
      offset += glm::ivec2(0, delta);
      updateTexture();
    }
    if (isKeyPressed(input, KEY_DOWN)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input) ? 10 : 1);
      offset += glm::ivec2(0, -delta);
      updateTexture();
    }
  }

  void render(const RunnerState& state) override {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUseProgram(shader_id);
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

 private:
  Vao vao = {{
      {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 2 * sizeof(f32)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  glm::ivec2 offset = {0, 0};
  GLuint texture_id;
  u32 perlin_seed = 1;
  f32 perlin_freq = 1;
  u32 perlin_oct = 1;
  GLuint shader_id;

  void updateTexture() {
    const auto width = runner().window().width;
    const auto height = runner().window().height;
    auto textureData = Buffer2d(width, height);
    perlinNoise(textureData, siv::PerlinNoise(perlin_seed), perlin_freq, perlin_oct, offset);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, &textureData);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  using namespace uinta;
  auto runner = GlfwRunner("Perlin", argc, argv);
  runner.window(Window(runner.window().title, 800, 800));
  if (auto error = runner.add_scene(std::make_unique<PerlinScene>(runner)); error) throw UintaException(error);
  return runner.run();
}
