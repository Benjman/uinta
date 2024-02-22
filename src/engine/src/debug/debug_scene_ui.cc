#include "uinta/debug/debug_scene_ui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "uinta/component.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/debug/ui/camera_ui.h"
#include "uinta/debug/ui/engine_ui.h"
#include "uinta/debug/ui/imgui.h"
#include "uinta/debug/ui/input_ui.h"
#include "uinta/debug/ui/platform_ui.h"
#include "uinta/debug/ui/primitive_shader_ui.h"
#include "uinta/debug/ui/scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/platform.h"

namespace uinta {

f32 UiSpacing = 0;
f32 UiWidth = 300;
f32 UiHalfWidth = UiWidth / 2.0;
f32 UiOneThirdsWidth = UiWidth * 1.0 / 3.0;
f32 UiTwoThirdsWidth = UiWidth * 2.0 / 3.0;
f32 UiOneQuartersWidth = UiWidth * 1.0 / 4.0;
f32 UiThreeQuartersWidth = UiWidth * 3.0 / 4.0;

static EngineUiInfo engineUiInfo_;
static f32 MaximumWindowHeight;

DebugSceneUi::DebugSceneUi(Scene* parent, Engine* engine,
                           const DebugSceneParams* params) noexcept
    : Scene(parent, engine, Layer::Debug),
      camera_(params->camera),
      primitiveShader_(params->shader),
      engine_(engine),
      gl_(engine->gl()) {
  engineUiInfo_.engine = engine;
  engine_->addListener<EngineEvent::RenderComplete>([&](const auto& event) {
    engineUiInfo_.render.push(event.duration);
    engineUiInfo_.frameDelta.push(event.state->frameDelta());
  });
  engine_->addListener<EngineEvent::TickComplete>([&](const auto& event) {
    engineUiInfo_.tick.push(event.duration);
    engineUiInfo_.delta.push(event.state->delta());
  });
  engine->addListener<EngineEvent::ViewportSizeChange>(
      [&](const auto& event) { MaximumWindowHeight = event.height; });
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
    UiThreeQuartersWidth -= UiSpacing * (3.0 / 4.0);

    ImGui_ImplGlfw_InitForOpenGL(
        static_cast<GLFWwindow*>(engine_->platform()->window()->userData()),
        true);
    ImGui_ImplOpenGL3_Init();
    flags_.isImGuiInitialized(true);
  }

  class FpsCounter : public NewFrameComponent {
   public:
    FpsCounter(EngineUiInfo::FpsCounterAvg*) noexcept {}

    void update(const EngineState& state) noexcept override {
      counter++;
      auto currentSecond = static_cast<std::size_t>(state.runtime());
      if (auto casted = static_cast<std::size_t>(state.runtime());
          currentSecond > prevSecond) {
        prevSecond = casted;
        engineUiInfo_.fps.push(counter);
        counter = 0;
      }
    }

   private:
    std::size_t counter = 0;
    std::size_t prevSecond = 0;
  };

  addComponent<FpsCounter>(&engineUiInfo_.fps);
}

void DebugSceneUi::preTick(const EngineState&) noexcept {
  if (flags_.isImGuiDeinitialized()) return;
  auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse) engine_->state().input().resetMouse();
  if (io.WantCaptureKeyboard) engine_->state().input().resetKeyboard();
}

void DebugSceneUi::postRender(const EngineState& state) noexcept {
  if (flags_.isImGuiNewFrame() && flags_.isImGuiDeinitialized()) return;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin(name().c_str());
  flags_.isImGuiNewFrame(true);
  flags_.isImGuiRendered(false);

  if (flags_.isImGuiRendered() && flags_.isImGuiDeinitialized()) return;
  RenderCameraUi(camera_);
  RenderEngineUi(engineUiInfo_);
  RenderImGuiMeta();
  RenderInputUi(state, engine_->platform()->window());
  RenderPlatformUi(engine_->platform());
  RenderPrimitiveShaderUi(primitiveShader_, gl_);
  RenderSceneUi(engine_, state);

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
