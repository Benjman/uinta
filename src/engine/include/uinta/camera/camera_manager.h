#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_

#include <utility>

#include "uinta/camera/camera_comparator.h"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_input_deltas.h"
#include "uinta/camera/camera_keybindings.h"
#include "uinta/component.h"
#include "uinta/engine/service_handle.h"

namespace uinta {

class CameraManager : public PreRenderComponent {
 public:
  explicit CameraManager(Engine* engine) noexcept;

  ~CameraManager() noexcept override;

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

  void update(time_t delta) noexcept override;

 private:
  ServiceHandle<CameraManager> serviceHandle_;
  Camera camera_;
  CameraComparator comparator_;
  CameraDispatchers dispatchers_;
  CameraInputDeltas computeDeltas(time_t delta) const noexcept;
  CameraKeybindings keybindings_;

  AppConfig* appConfig_;
  const Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
