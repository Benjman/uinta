#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_

#include "uinta/flags.h"
#include "uinta/scene/scene.h"

namespace uinta {

class BasicShaderManager;
class CameraManager;
class Engine;
class Input;
class OpenGLApi;
class ViewportManager;

class DebugSceneUi : public Scene {
 public:
  explicit DebugSceneUi(Scene*) noexcept;
  ~DebugSceneUi() noexcept override;

  void preTick(time_t /*unused*/) noexcept override;
  void render(time_t /*unused*/) noexcept override;

 private:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(0);

    [[nodiscard]] bool isImGuiInitialized() const noexcept {
      return (flags_ & ImGuiInitializedMask) != 0;
    }
    void isImGuiInitialized(bool v) noexcept {
      flags_ &= ~ImGuiInitializedMask;
      if (v) {
        flags_ |= ImGuiInitializedMask;
      }
    }

    [[nodiscard]] bool isImGuiDeinitialized() const noexcept {
      return (flags_ & ImGuiDeinitializedMask) != 0;
    }
    void isImGuiDeinitialized(bool v) noexcept {
      flags_ &= ~ImGuiDeinitializedMask;
      if (v) {
        flags_ |= ImGuiDeinitializedMask;
      }
    }

    [[nodiscard]] bool isImGuiNewFrame() const noexcept {
      return (flags_ & ImGuiNewFrameMask) != 0;
    }
    void isImGuiNewFrame(bool v) noexcept {
      flags_ &= ~ImGuiNewFrameMask;
      if (v) {
        flags_ |= ImGuiNewFrameMask;
      }
    }

    [[nodiscard]] bool isImGuiRendered() const noexcept {
      return (flags_ & ImGuiRenderedMask) != 0;
    }
    void isImGuiRendered(bool v) noexcept {
      flags_ &= ~ImGuiRenderedMask;
      if (v) {
        flags_ |= ImGuiRenderedMask;
      }
    }

   private:
    static constexpr value_type ImGuiInitializedMask = 1 << 0;
    static constexpr value_type ImGuiDeinitializedMask = 1 << 1;
    static constexpr value_type ImGuiNewFrameMask = 1 << 2;
    static constexpr value_type ImGuiRenderedMask = 1 << 3;

    value_type flags_;
  } flags_;

  time_t runtime_ = 0;

  ViewportManager* viewport_ = nullptr;
  CameraManager* camera_ = nullptr;
  BasicShaderManager* shader_;
  Input* input_ = nullptr;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_UI_H_
