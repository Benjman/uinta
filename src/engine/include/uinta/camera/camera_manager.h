#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_

#include <utility>

#include "uinta/camera/camera_comparator.h"
#include "uinta/camera/camera_events.h"
#include "uinta/system.h"

namespace uinta {

class Camera;

class CameraManager : public System {
 public:
  explicit CameraManager(Camera* camera) noexcept : camera_(camera) {}

  const Camera* camera() const noexcept { return camera_; }

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  void onTick(const EngineState& state) noexcept override {
    camera_->update(state);
  }

  void onNewFrame(const EngineState&) noexcept override {
    if (comparator_ == camera_) return;
    comparator_ = *camera_;
    dispatchers_.dispatch<CameraEvent::ViewMatrixUpdated>(
        ViewMatrixUpdateEvent(camera_, camera_->viewMatrix()));
  }

  CameraDispatchers* dispatchers() noexcept { return &dispatchers_; }

 private:
  CameraComparator comparator_;
  CameraDispatchers dispatchers_;

  Camera* camera_;

  template <CameraEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    dispatchers_.template dispatch<E>(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
