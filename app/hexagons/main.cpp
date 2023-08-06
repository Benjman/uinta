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
    scene.camera.dist = 45;
    scene.camera.pitch = 90;
  }

  bool doInit() override {
    if (!GlfwRunner::doInit()) return false;

    constexpr auto gridRadius = 15;
    constexpr auto hexSize = 1.0;
    constexpr auto gridCenter = glm::ivec3(0);
    const auto points = radial_hexagons(gridCenter, gridRadius, hexSize);

    srand(time(nullptr));  // rand() seed
    auto colors = std::vector<glm::vec3>(points.size() / VerticesPerHex);
    for (size_t i = 0; i < colors.size(); ++i) colors.at(i) = glm::vec3(0.21, 0.38, 0.21) + 0.03f * static_cast<f32>(rand() % 4);

    const auto hexCount = hexagon_count(gridRadius);
    f32 vtxBuffer[VerticesPerHex * hexCount * 9];
    u32 idxBuffer[IndicesPerHex * hexCount];
    auto idxOffset = 0u;
    hexagon_pack(points, colors, vtxBuffer, idxBuffer, idxOffset);

    initVao(vao);
    uploadVbo(vbo, vtxBuffer, sizeof(vtxBuffer));
    initVertexAttribs(vao);
    indexBuffer(vao, idxBuffer, sizeof(idxBuffer));
    indexCount = IndicesPerHex * hexCount;

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
