#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_

#include <type_traits>
#include <typeindex>
#include <utility>

namespace uinta {

class Engine;

namespace _ {

// Type-erased forwarding to `Engine`, defined in engine.cc, so this header does not depend on engine.h.
void registerService(Engine* engine, std::type_index type, void* service, bool isConst) noexcept;

void unregisterService(Engine* engine, std::type_index type) noexcept;

}  // namespace _

template <typename T>
class ServiceHandle {
 public:
  // A null `engine` yields an inert handle, allowing services to be constructed standalone (e.g. in tests).
  ServiceHandle(Engine* engine, T* service) noexcept : engine_(engine) {
    if (engine_ == nullptr) {
      return;
    }
    _::registerService(engine_, std::type_index(typeid(T)), const_cast<void*>(static_cast<const void*>(service)),
                       std::is_const_v<T>);
  }

  ~ServiceHandle() noexcept {
    if (engine_ != nullptr) {
      unregister();
    }
  }

  ServiceHandle(const ServiceHandle&) = delete;
  ServiceHandle& operator=(const ServiceHandle&) = delete;

  ServiceHandle(ServiceHandle&& other) noexcept : engine_(std::exchange(other.engine_, nullptr)) {}

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
  void unregister() noexcept { _::unregisterService(engine_, std::type_index(typeid(T))); }

  Engine* engine_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_HANDLE_H_
