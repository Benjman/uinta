#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_

#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <unordered_map>

namespace uinta {

class Engine;

class ServiceRegistry {
 public:
  ServiceRegistry() noexcept = default;
  ~ServiceRegistry() noexcept = default;

  ServiceRegistry(const ServiceRegistry&) = delete;
  ServiceRegistry& operator=(const ServiceRegistry&) = delete;
  ServiceRegistry(ServiceRegistry&&) = delete;
  ServiceRegistry& operator=(ServiceRegistry&&) = delete;

  template <typename T>
  void registerService(T* service) noexcept {
    std::unique_lock lock(mutex_);
    auto key = std::type_index(typeid(T));
    assert(!services_.contains(key) && "Service already registered");
    services_[key] = static_cast<void*>(service);
  }

  template <typename T>
  void unregisterService() noexcept {
    std::unique_lock lock(mutex_);
    auto key = std::type_index(typeid(T));
    services_.erase(key);
  }

  template <typename T>
  T* service() noexcept {
    std::shared_lock lock(mutex_);
    auto key = std::type_index(typeid(T));
    auto it = services_.find(key);
    if (it == services_.end()) {
      return nullptr;
    }
    return static_cast<T*>(it->second);
  }

  template <typename T>
  const T* service() const noexcept {
    std::shared_lock lock(mutex_);
    auto key = std::type_index(typeid(T));
    auto it = services_.find(key);
    if (it == services_.end()) {
      return nullptr;
    }
    return static_cast<const T*>(it->second);
  }

 private:
  mutable std::shared_mutex mutex_;
  std::unordered_map<std::type_index, void*> services_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_
