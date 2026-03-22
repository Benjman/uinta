#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_

#include <bitset>
#include <utility>
#include <vector>

#include "uinta/camera/camera_comparator.h"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_input_deltas.h"
#include "uinta/camera/camera_keybindings.h"
#include "uinta/engine/service_handle.h"
#include "uinta/input/input_token.h"
#include "uinta/types.h"

namespace uinta {

class AppConfig;
class Engine;
class InputSystem;

class CameraManager {
 public:
  explicit CameraManager(Engine* engine) noexcept;

  ~CameraManager() noexcept;

  CameraManager(const CameraManager&) = delete;
  CameraManager(CameraManager&&) = delete;
  CameraManager& operator=(const CameraManager&) = delete;
  CameraManager& operator=(CameraManager&&) = delete;

  Camera* camera() noexcept { return &camera_; }

  const Camera* camera() const noexcept { return &camera_; }

  CameraKeybindings& keybindings() noexcept { return keybindings_; }

  const CameraKeybindings& keybindings() const noexcept { return keybindings_; }

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  void update(time_t delta) noexcept;

 private:
  ServiceHandle<CameraManager> serviceHandle_;
  Camera camera_;
  CameraComparator comparator_;
  CameraDispatchers dispatchers_;
  CameraInputDeltas computeDeltas(time_t delta) const noexcept;
  CameraKeybindings keybindings_;

  AppConfig* appConfig_;
  InputSystem* inputSystem_;
  std::vector<SubscriptionHandle> subscriptions_;

  std::bitset<static_cast<size_t>(Key::Menu) + 1> keysDown_;
  std::bitset<static_cast<size_t>(MouseBtn::Button8) + 1> mouseDown_;
  f32 cursordx_ = 0;
  f32 cursordy_ = 0;
  f32 scrolldy_ = 0;

  void subscribe() noexcept;

  void subscribeKey(Key key) noexcept;

  void subscribeMouse(MouseBtn button) noexcept;

  bool isKeyDown(Key key) const noexcept { return key != Key::Unspecified && keysDown_.test(static_cast<size_t>(key)); }

  bool isMouseDown(MouseBtn button) const noexcept {
    return button != MouseBtn::Unspecified && mouseDown_.test(static_cast<size_t>(button));
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
