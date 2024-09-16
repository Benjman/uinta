#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_

#include <utility>

#include "uinta/camera/camera_comparator.h"
#include "uinta/camera/camera_events.h"
#include "uinta/system.h"

namespace uinta {

class AppConfig;
class Scene;

class CameraManager : public System {
 public:
  explicit CameraManager(Scene*) noexcept;

  ~CameraManager() noexcept;

  CameraManager(const CameraManager&) = delete;
  CameraManager(CameraManager&&) = delete;
  CameraManager& operator=(const CameraManager&) = delete;
  CameraManager& operator=(CameraManager&&) = delete;

  Camera* camera() noexcept { return &camera_; }

  const Camera* camera() const noexcept { return &camera_; }

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  void onTick(time_t delta) noexcept override { camera_.update(delta, input_); }

  void onPreRender(time_t) noexcept override {
    if (comparator_ == &camera_) return;
    comparator_ = camera_;
    dispatchers_.dispatch<CameraEvent::ViewMatrixUpdated>(
        ViewMatrixUpdateEvent(&camera_, camera_.viewMatrix()));
  }

  auto orthoSize() const noexcept { return orthoSize_; }

  void projection(CameraProjection projection) noexcept {
    if (projection == camera_.config().projection) return;
    camera_.config().projection = projection;
    auto mat = camera_.config().projection == CameraProjection::Perspective
                   ? camera_.perspectiveMatrix(aspect_)
                   : camera_.orthographicMatrix(aspect_);
    dispatchers_.dispatch<CameraEvent::ProjectionMatrixUpdated>(
        ProjectionMatrixUpdateEvent(&camera_, mat, aspect_));
  }

 private:
  CameraComparator comparator_;
  CameraDispatchers dispatchers_;
  Camera camera_;
  f32 aspect_ = 1;
  f32 orthoSize_;

  AppConfig* appConfig_;
  const Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_MANAGER_H_
