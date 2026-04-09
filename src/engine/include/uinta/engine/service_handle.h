#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_

#include <utility>

#include "uinta/engine/engine.h"

namespace uinta {

class Engine;

template <typename T>
class ServiceHandle {
 public:
  ServiceHandle(Engine* engine, T* service) noexcept : engine_(engine) {
    engine_->registerService<T>(service);
  }

  ~ServiceHandle() noexcept {
    if (engine_ != nullptr) {
      unregister();
    }
  }

  ServiceHandle(const ServiceHandle&) = delete;
  ServiceHandle& operator=(const ServiceHandle&) = delete;

  ServiceHandle(ServiceHandle&& other) noexcept
      : engine_(std::exchange(other.engine_, nullptr)) {}

  ServiceHandle& operator=(ServiceHandle&& other) noexcept {
    if (this != &other) {
      if (engine_ != nullptr) {
        unregister();
      }
      engine_ = std::exchange(other.engine_, nullptr);
    }
    return *this;
  }

 private:
  void unregister() noexcept { engine_->unregisterService<T>(); }

  Engine* engine_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_
