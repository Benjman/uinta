#ifndef UINTA_RESOURCE_HPP
#define UINTA_RESOURCE_HPP

#include <uinta/logging.hpp>

#include <limits>   // std::numeric_limits
#include <stdint.h> // uint32_t
#include <string>   // std::string
#include <vector>   // std::vector

namespace uinta {
namespace resource {

inline const uint16_t UINTA_RES_MAX_BUFFERS        = 256;
inline const uint16_t UINTA_RES_ID_MASK            = 0xFFFF;
inline const uint16_t UINTA_RES_HANDLE_T_MAX_VALUE = 65535;

using resource_handle_t = uint32_t;

struct ResourceHandle {
  ResourceHandle() : ResourceHandle(0) {}
  ResourceHandle(const resource_handle_t handle) : handle(handle) {}
  ResourceHandle(const ResourceHandle &other) { *this = other; }

  ResourceHandle &operator=(const ResourceHandle &other) {
    setId(other.handle);
    return *this;
  }

  void setId(const ResourceHandle &handle) { setId(handle.handle); }
  void setId(const resource_handle_t handle) { this->handle = handle & UINTA_RES_ID_MASK; }

  resource_handle_t getId() const { return handle & UINTA_RES_ID_MASK; }

private:
  resource_handle_t handle : 32;
};

class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager();

  ResourceHandle *registerResource(const std::string &relativePath);

  void loadResource(const ResourceHandle *handle);
  uint32_t getSize(const ResourceHandle *handle);

private:
  std::string paths[UINTA_RES_MAX_BUFFERS];
  uint16_t pathsCount;
  uint32_t sizes[UINTA_RES_MAX_BUFFERS];
  uint16_t flags[UINTA_RES_MAX_BUFFERS];

  logger_t logger;

  ResourceHandle generateHandle();
};

} // namespace resource
} // namespace uinta

#endif // UINTA_RESOURCE_HPP
