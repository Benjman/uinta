#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "uinta/camera/camera_ui.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/debug/ui/engine_state_ui.h"
#include "uinta/debug/ui/input_capture_watcher.h"
#include "uinta/engine.h"
#include "uinta/platform.h"
#include "uinta/scene.h"

namespace uinta {

class DebugSceneUi : public Scene {
  using Flags = u32;

 public:
  DebugSceneUi(Scene* parent, const DebugSceneParams& params) noexcept
      : Scene(parent, Layer::Debug),
        engineStateUi_(),
        cameraUi_(params.camera) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(
        static_cast<GLFWwindow*>(
            params.engine->platform()->window()->userData()),
        true);
    ImGui_ImplOpenGL3_Init();
    addSystem<InputCaptureWatcher>(&params.engine->state().input());
  }

  void preRender(const EngineState&) noexcept override {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void render(const EngineState& state) noexcept override {
    if (ImGui::TreeNode("Camera")) {
      cameraUi_.render(state);
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Engine state")) {
      engineStateUi_.render(state);
      ImGui::TreePop();
    }
  }

  void postRender(const EngineState&) noexcept override {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  ~DebugSceneUi() noexcept {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

 private:
  EngineStateUi engineStateUi_;
  CameraUi cameraUi_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
