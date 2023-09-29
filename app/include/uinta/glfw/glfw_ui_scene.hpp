#ifndef UINTA_GLFW_UI_SCENE_HPP
#define UINTA_GLFW_UI_SCENE_HPP

#include <uinta/flags.h>

#include <uinta/scene.hpp>

namespace uinta {

class GlfwRunner;

class GlfwRunnerUi : public Scene {
 public:
  enum Flags : flag_t {
    CaptureMouse = 1 << 0,
    CaptureKeyboard = 1 << 1,
  };

  GlfwRunnerUi(GlfwRunner &runner);

  uinta_error_code init() override;

  void pre_tick(const RunnerState &state, const InputState &input) override;

  void tick(const RunnerState &state, const InputState &input) override;

  void post_tick(const RunnerState &state, const InputState &input) override;

  void pre_render(const RunnerState &state) override;

  void render(const RunnerState &state) override;

  void post_render(const RunnerState &state) override;

  void shutdown() override;

 private:
  flags_t m_flags = 0;
};

}  // namespace uinta

#endif  // UINTA_GLFW_UI_SCENE_HPP
