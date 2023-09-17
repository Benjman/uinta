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
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::HexagonsRunner(argc, argv).run();
}
