#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_

#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
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
    services_[key] = Entry{
        .ptr = const_cast<void*>(static_cast<const void*>(service)),
        .isConst = std::is_const_v<T>,
    };
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
    // A service registered as `const T` must not be handed out as a mutable `T*`.
    if (it->second.isConst && !std::is_const_v<T>) {
      return nullptr;
    }
    return static_cast<T*>(it->second.ptr);
  }

  template <typename T>
  const T* service() const noexcept {
    std::shared_lock lock(mutex_);
    auto key = std::type_index(typeid(T));
    auto it = services_.find(key);
    if (it == services_.end()) {
      return nullptr;
    }
    return static_cast<const T*>(it->second.ptr);
  }

 private:
  // `typeid` strips cv-qualifiers, so `T` and `const T` share a key; `isConst` records how it was registered.
  struct Entry {
    void* ptr;
    bool isConst;
  };

  mutable std::shared_mutex mutex_;
  std::unordered_map<std::type_index, Entry> services_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_SERVICE_REGISTRY_H_
