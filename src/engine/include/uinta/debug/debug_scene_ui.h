#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_

#include <string>

#include "uinta/debug/debug_scene.h"
#include "uinta/flags.h"
#include "uinta/scene.h"

namespace uinta {

class Camera;
class Engine;
class OpenGLApi;
class PrimitiveShader;

class DebugSceneUi : public Scene {
 public:
  DebugSceneUi(Scene* parent, Engine* engine,
               const DebugSceneParams* params) noexcept;
  ~DebugSceneUi() noexcept;

  std::string name() const noexcept override { return "DebugSceneUi"; }

  void preTick(const EngineState&) noexcept override;
  void postRender(const EngineState&) noexcept override;

 private:
  struct Flags final {
    using value_type = u8;

    AtomicFlagsOperations(0);

    bool isImGuiInitialized() const noexcept {
      return flags_ & ImGuiInitializedMask;
    }
    void isImGuiInitialized(bool v) noexcept {
      flags_ &= ~ImGuiInitializedMask;
      if (v) flags_ |= ImGuiInitializedMask;
    }

    bool isImGuiDeinitialized() const noexcept {
      return flags_ & ImGuiDeinitializedMask;
    }
    void isImGuiDeinitialized(bool v) noexcept {
      flags_ &= ~ImGuiDeinitializedMask;
      if (v) flags_ |= ImGuiDeinitializedMask;
    }

    bool isImGuiNewFrame() const noexcept { return flags_ & ImGuiNewFrameMask; }
    void isImGuiNewFrame(bool v) noexcept {
      flags_ &= ~ImGuiNewFrameMask;
      if (v) flags_ |= ImGuiNewFrameMask;
    }

    bool isImGuiRendered() const noexcept { return flags_ & ImGuiRenderedMask; }
    void isImGuiRendered(bool v) noexcept {
      flags_ &= ~ImGuiRenderedMask;
      if (v) flags_ |= ImGuiRenderedMask;
    }

   private:
    static constexpr value_type ImGuiInitializedMask = 1 << 0;
    static constexpr value_type ImGuiDeinitializedMask = 1 << 1;
    static constexpr value_type ImGuiNewFrameMask = 1 << 2;
    static constexpr value_type ImGuiRenderedMask = 1 << 3;

    std::atomic<value_type> flags_;
  } flags_;

  Camera* camera_;
  PrimitiveShader* primitiveShader_;

  Engine* engine_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
