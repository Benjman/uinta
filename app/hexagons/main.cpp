#include <uinta/error.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/math/hexagon.hpp>
#include <uinta/math/perlin.hpp>

namespace uinta {

class HexagonsRunner : public GlfwRunner {
  static constexpr auto yScale = 15;

 public:
  HexagonsRunner(const i32 argc, const char** argv) : GlfwRunner("Hexagons", argc, argv) {
    if (isFlagSet(Scene::CAMERA_ENABLED, scene().flags())) {
      auto camera = scene().camera();
      camera.angle(12);
      camera.pitch(25);
      camera.dist(56);
      camera.vert_offset(yScale * 0.5);
      auto config = camera.config();
      config.dst_max = 1e6;
      camera.config(config);
      scene().camera(camera);
    }
    scene().diffuse_light({{0.5, -1, 0}});
  }

 private:
  Vao vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 3 * sizeof(f32)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 6 * sizeof(f32)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  u32 indexCount;

  uinta_error_code doInit() override {
    if (auto error = GlfwRunner::doInit(); error) return error;

    constexpr auto hexRadius = 1.0;

    /// Generate vertex points for a radial hexagon grid:
    constexpr auto gridRings = 20;
    constexpr auto centerPoint = glm::ivec3(0);
    const auto spacing = hex_spacing(hexRadius);
    auto points = radial_hexagons(centerPoint, gridRings, hexRadius);

    /// Set height values for the vertex points based on the height map:
    if (yScale) {
      const auto heightMapSize = glm::ivec2(std::ceil((gridRings + 1) * spacing.x * (gridRings + 1) * spacing.y));
      auto heightMap = Buffer2d(heightMapSize);
      perlinNoise(heightMap, siv::PerlinNoise(), 40, 1);
      hex_set_heights(points, heightMap, yScale);
    }

    /// Add some fun colors:
    srand(time(nullptr));  /// rand() seed
    auto colors = std::vector<glm::vec3>(hexagon_count(gridRings));
    for (auto& color : colors) color = glm::vec3(0.21, 0.38, 0.21) + 0.015f * static_cast<float>(rand() % 4);

    /// Prepare GPU-bound buffers:
    constexpr auto elementsPerVertex = 9;
    f32 vtxBuffer[hexagon_count(gridRings) * VerticesPerHex * elementsPerVertex];
    u32 idxBuffer[hexagon_count(gridRings) * IndicesPerHex];
    u32 idxOffset = 0;
    hexagon_pack(points, hex_normals(points), colors, vtxBuffer, idxBuffer, idxOffset);

    // Upload buffers to GPU:
    vao.init();
    vao.index_buffer().upload(idxBuffer, sizeof(idxBuffer), 0);
    vbo.init();
    vbo.upload(vtxBuffer, sizeof(vtxBuffer), 0);
    vao.init_attributes();
    indexCount = IndicesPerHex * points.size();

    return SUCCESS_EC;
  }

  void doRender() override {
    GlfwRunner::doRender();
    glm::mat4 view = scene().camera().view_matrix();
    glm::mat4 projection = scene().camera().perspective_matrix();
    scene().renderer().start(state(), view, projection);
    vao.bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0L));
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::HexagonsRunner(argc, argv).run();
}
