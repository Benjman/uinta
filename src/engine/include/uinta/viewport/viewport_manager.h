#ifndef SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_MANAGER_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <utility>

#include "uinta/viewport/viewport_config.h"
#include "uinta/viewport/viewport_events.h"

namespace uinta {

class AppConfig;
class Engine;

class ViewportManager {
 public:
  explicit ViewportManager(Engine* engine, AppConfig* appConfig) noexcept;

  ~ViewportManager() noexcept;

  ViewportManager(const ViewportManager&) = delete;
  ViewportManager(ViewportManager&&) = delete;
  ViewportManager& operator=(const ViewportManager&) = delete;
  ViewportManager& operator=(ViewportManager&&) = delete;

  [[nodiscard]] glm::mat4 projectionMatrix() const noexcept;

  [[nodiscard]] const ViewportConfig& config() const noexcept {
    return config_;
  }

  ViewportConfig& config() noexcept { return config_; }

  [[nodiscard]] f32 aspect() const noexcept { return aspect_; }

  [[nodiscard]] ProjectionType projection() const noexcept {
    return config_.projection;
  }

  void projection(ProjectionType projection) noexcept;

  [[nodiscard]] f32 fov() const noexcept { return config_.fov; }

  void fov(f32 fov) noexcept;

  [[nodiscard]] f32 near() const noexcept { return config_.near; }

  void near(f32 near) noexcept;

  [[nodiscard]] f32 far() const noexcept { return config_.far; }

  void far(f32 far) noexcept;

  [[nodiscard]] f32 orthoSize() const noexcept { return config_.orthoSize; }

  void orthoSize(f32 orthoSize) noexcept;

  template <ViewportEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

 private:
  void dispatchProjectionUpdate() noexcept;

  ViewportDispatchers dispatchers_;
  ViewportConfig config_;
  f32 aspect_ = 1.0f;

  AppConfig* appConfig_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_MANAGER_H_
