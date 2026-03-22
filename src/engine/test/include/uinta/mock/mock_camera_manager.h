#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_CAMERA_MANAGER_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_CAMERA_MANAGER_H_

#include <utility>

#include "uinta/camera/camera.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_events.h"

namespace uinta {

struct MockCameraManager {
  MockCameraManager() noexcept : camera_(CameraConfig{}) {}

  Camera* camera() noexcept { return &camera_; }

  const Camera* camera() const noexcept { return &camera_; }

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  template <CameraEvent E, typename... Args>
  void dispatch(Args&&... args) noexcept {
    dispatchers_.template dispatch<E>(std::forward<Args>(args)...);
  }

 private:
  CameraDispatchers dispatchers_;
  Camera camera_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_CAMERA_MANAGER_H_
