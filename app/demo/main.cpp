#include <uinta/error.hpp>
#include <uinta/glfw/glfw_runner.hpp>

namespace uinta {

class DemoRunner : public GlfwRunner {
 public:
  entt::entity entity;

  DemoRunner(const int argc, const char** argv) : GlfwRunner("Demo", argc, argv) {
  }

  uinta_error_code doInit() override {
    if (auto error = GlfwRunner::doInit(); error) return error;

    // Notice that there is an issue translating vertices from Blender into the engine where Blender's Y-plane is mapping to our
    // Z-plane. Though not intentional, it seemingly works because if we exported from Blender to correctly map to our xyz
    // coordinates, it would make modeling in Blender wonky because everything would be sideways-up.
    if (auto error = scene.addEntity(entity, {"models/xyz.obj"}, registry); error) return error;
    registry.get<Transform>(entity).scale *= 0.5;

    if (isFlagSet(Scene::CAMERA_ENABLED, scene.flags)) {
      force(scene.camera.angle, 45);
      force(scene.camera.pitch, 45);
      force(scene.camera.dist, 7);
    }

    scene.updateDiffuseLight({{-1.5, 2, 1}});

    return SUCCESS_EC;
  }

  void doRender(const RunnerState& state) override {
    GlfwRunner::doRender(state);
    scene.startRender(state);
    scene.renderEntity(entity, registry);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner(argc, argv).run();
}
