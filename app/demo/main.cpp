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
    if (auto error = scene().addEntity(entity, file_manager(), model_manager(), {"models/xyz.obj"}); error) return error;
    scene().registry().get<Transform>(entity).scale *= 0.5;

    scene().diffuse_light({{-1.5, 2, 1}});
    if (isFlagSet(Scene::CAMERA_ENABLED, scene().flags())) {
      auto camera = scene().camera();
      camera.angle(45);
      camera.pitch(45);
      camera.dist(7);
      scene().camera(camera);
    }

    return SUCCESS_EC;
  }

  void doRender() override {
    GlfwRunner::doRender();
    scene().renderEntity(entity);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::DemoRunner(argc, argv).run();
}
