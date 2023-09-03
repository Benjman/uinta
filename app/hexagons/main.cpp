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
      camera.angle = 12;
      camera.pitch = 8;
      camera.dist = 56;
      camera.config.dst_max = 1e6;
      camera.vertOffset = yScale * 0.5;
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

    /// Construct and fill height map
    constexpr auto gridRings = 20;
    const auto spacing = hex_spacing(hexRadius);
    const auto heightMapSize = glm::ivec2(std::ceil((gridRings + 1) * spacing.x * (gridRings + 1) * spacing.y));
    auto heightMap = Buffer2d(heightMapSize);
    perlinNoise(heightMap, siv::PerlinNoise(), 40, 1);

    /// Generate vertex points for a radial hexagon grid:
    constexpr auto centerPoint = glm::ivec3(0);
    auto points = radial_hexagons(centerPoint, gridRings, hexRadius);

    /// Set height values for the vertex points based on the height map:
    hex_set_heights(points, heightMap, yScale);

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
    if (auto error = initVao(vao); error) return error;
    if (auto error = uploadVbo(vbo, vtxBuffer, sizeof(vtxBuffer)); error) return error;
    if (auto error = initVertexAttribs(vao); error) return error;
    if (auto error = indexBuffer(vao, idxBuffer, sizeof(idxBuffer)); error) return error;
    indexCount = IndicesPerHex * points.size();

    return SUCCESS_EC;
  }

  void doRender(const RunnerState& state) override {
    GlfwRunner::doRender(state);
    scene().renderer().start(getViewMatrix(scene().camera()), getPerspectiveMatrix(scene().camera()), state);
    bindVao(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0L));
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::HexagonsRunner(argc, argv).run();
}
