#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/math/hexagon.hpp>

namespace uinta {

class HexagonsRunner : public GlfwRunner {
 public:
  Vao vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 3 * sizeof(f32)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 6 * sizeof(f32)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  u32 indexCount;

  HexagonsRunner(const i32 argc, const char** argv) : GlfwRunner("Hexagons", argc, argv) {
    setFlag(TargetCamera::CAMERA_DIST_LIMIT, false, scene.camera.flags);
    setFlag(Runner::GRID_ENABLED, false, flags);
    scene.camera.dist = 30;
    scene.camera.pitch = 90;
  }

  bool doInit() override {
    if (!GlfwRunner::doInit()) return false;

    constexpr auto elePerHex = 9;
    constexpr auto gridWidth = 22;
    constexpr auto gridHeight = 15;
    f32 vtxBuffer[VerticesPerHex * gridWidth * gridHeight * elePerHex];
    u32 idxBuffer[IndicesPerHex * gridWidth * gridHeight];
    u32 idxOffset = 0;

    constexpr auto gridCenter = glm::vec2(0);
    constexpr auto hexSize = 1u;
    const auto spacing = hex_spacing(hexSize);
    srand(time(nullptr));  // rand() seed
    force(scene.camera.target = {spacing.x * gridWidth * 0.5, 0, spacing.y * gridHeight * 0.5});

    for (auto h = 0; h < gridHeight; ++h) {
      for (auto w = 0; w < gridWidth; ++w) {
        auto center = glm::vec2(w * spacing.x, h * spacing.y) + gridCenter;
        if (h % 2) center.x += spacing.x * 0.5;
        const auto points = hex_points(center, hexSize);
        const auto colors = std::vector<glm::vec3>(1) = {glm::vec3(0.21, 0.38, 0.21) + 0.03f * static_cast<f32>(rand() % 4)};
        auto* vtxPtr = &vtxBuffer[VerticesPerHex * (w + h * gridWidth) * elePerHex];
        auto* idxPtr = &idxBuffer[IndicesPerHex * (w + h * gridWidth)];
        hexagon_pack(std::vector<glm::vec3>(points.begin(), points.end()), colors, vtxPtr, idxPtr, idxOffset);
      }
    }

    initVao(vao);
    uploadVbo(vbo, vtxBuffer, sizeof(vtxBuffer));
    initVertexAttribs(vao);
    indexBuffer(vao, idxBuffer, sizeof(idxBuffer));
    indexCount = IndicesPerHex * gridWidth * gridHeight;

    return true;
  }

  void doRender(const RunnerState& state) override {
    GlfwRunner::doRender(state);
    scene.shader.start(getViewMatrix(scene.camera), getPerspectiveMatrix(scene.camera), state);
    bindVao(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0L));
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::HexagonsRunner(argc, argv).run();
}
