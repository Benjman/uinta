#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_

#include "uinta/flags.h"
#include "uinta/scene/scene.h"

namespace uinta {

class BasicShaderManager;
class Engine;
class OpenGLApi;

class DebugSceneUi : public Scene {
 public:
  explicit DebugSceneUi(Engine*) noexcept;
  explicit DebugSceneUi(Scene*) noexcept;
  ~DebugSceneUi() noexcept;

  void render(time_t) noexcept override;

 private:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(0);

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

    value_type flags_;
  } flags_;

  time_t runtime_ = 0;

  BasicShaderManager* shader_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
