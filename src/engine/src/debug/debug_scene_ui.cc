#include "uinta/debug/debug_scene_ui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>

#include "uinta/component.h"
#include "uinta/debug/ui/basic_shader_ui.h"
#include "uinta/debug/ui/engine_ui.h"
#include "uinta/debug/ui/imgui_ui.h"
#include "uinta/debug/ui/input_ui.h"
#include "uinta/debug/ui/platform_ui.h"
#include "uinta/debug/ui/scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/platform.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

f32 UiSpacing = 0;
f32 UiWidth = 300;
f32 UiHalfWidth = UiWidth / 2.0;
f32 UiOneThirdsWidth = UiWidth * 1.0 / 3.0;
f32 UiTwoThirdsWidth = UiWidth * 2.0 / 3.0;
f32 UiOneQuartersWidth = UiWidth * 1.0 / 4.0;
f32 UiOneFifthsWidth = UiWidth * 1.0 / 5.0;
f32 UiThreeQuartersWidth = UiWidth * 3.0 / 4.0;

static EngineUiInfo engineUiInfo_;

DebugSceneUi::DebugSceneUi(Scene* scene) noexcept
    : DebugSceneUi(scene ? scene->engine() : nullptr) {}

DebugSceneUi::DebugSceneUi(Engine* engine) noexcept
    : Scene(engine, SCENE_NAME, SceneLayer::Debug),
      shader_(findSystem<BasicShaderManager>().value_or(nullptr)),
      input_(engine->input()) {
  static time_t prevRenderComplete = 0;
  engineUiInfo_.engine = engine;
  engine->dispatchers()->addListener<EngineEvent::RenderComplete>(
      [](const RenderComplete& event) {
        engineUiInfo_.render.push(event.runtime - prevRenderComplete);
        prevRenderComplete = event.runtime;
      });

  static time_t prevTickComplete = 0;
  engine->dispatchers()->addListener<EngineEvent::TickComplete>(
      [](const TickComplete& event) {
        engineUiInfo_.tick.push(event.runtime - prevTickComplete);
        engineUiInfo_.delta.push(event.state->delta());
        prevTickComplete = event.runtime;
      });

  if (!flags_.isImGuiInitialized()) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    auto& style = ImGui::GetStyle();
    style.WindowMinSize = ImVec2(
        UiWidth + style.DisplayWindowPadding.x * 2 + style.ScrollbarSize, 400);
    style.WindowRounding = 8;
    style.ScrollbarRounding = 4;

    UiSpacing = style.ItemSpacing.x;
    UiOneThirdsWidth -= UiSpacing * (1.0 / 3.0);
    UiTwoThirdsWidth -= UiSpacing * (2.0 / 3.0);
    UiOneQuartersWidth -= UiSpacing * (1.0 / 4.0);
    UiOneFifthsWidth -= UiSpacing * (1.0 / 5.0);
    UiThreeQuartersWidth -= UiSpacing * (3.0 / 4.0);

    ImGui_ImplGlfw_InitForOpenGL(
        static_cast<GLFWwindow*>(engine->platform()->window()->userData()),
        true);
    ImGui_ImplOpenGL3_Init();
    flags_.isImGuiInitialized(true);
  }

  class FpsCounter : public PostRenderComponent {
   public:
    FpsCounter(EngineUiInfo::FpsCounterAvg*) noexcept {}

    void update(time_t delta) noexcept override {
      counter++;
      runtime_ += delta;
      auto currentSecond = static_cast<size_t>(runtime_);
      if (auto casted = static_cast<size_t>(runtime_);
          currentSecond > prevSecond) {
        prevSecond = casted;
        engineUiInfo_.fps.push(counter);
        counter = 0;
      }
    }

   private:
    time_t counter = 0;
    time_t prevSecond = 0;
    time_t runtime_;
  };

  addComponent<FpsCounter>(&engineUiInfo_.fps);
}

void DebugSceneUi::preTick(time_t) noexcept {
  if (flags_.isImGuiDeinitialized()) return;
  auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse) engine()->input()->resetMouse();
  if (io.WantCaptureKeyboard) engine()->input()->resetKeyboard();
}

void DebugSceneUi::render(time_t) noexcept {
  if (flags_.isImGuiNewFrame() && flags_.isImGuiDeinitialized()) return;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin(std::string("Menu: " + name()).c_str());

  flags_.isImGuiNewFrame(true);
  flags_.isImGuiRendered(false);

  if (flags_.isImGuiRendered() && flags_.isImGuiDeinitialized()) return;

  if (shader_) {
    RenderBasicShaderUi(shader_, engine()->gl());
  }

  RenderEngineUi(engineUiInfo_);
  RenderImGuiUi(engine()->platform()->window());
  RenderInputUi(input_, engine()->platform()->window());
  RenderPlatformUi(engine()->platform());
  RenderSceneUi(engine());

  if (flags_.isImGuiDeinitialized()) return;
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  flags_.isImGuiRendered(true);
  flags_.isImGuiNewFrame(false);
}

DebugSceneUi::~DebugSceneUi() noexcept {
  if (flags_.isImGuiDeinitialized()) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  flags_.isImGuiDeinitialized(true);
}

}  // namespace uinta
