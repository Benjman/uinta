#ifndef UINTA_RESOURCE_HPP
#define UINTA_RESOURCE_HPP

#include <uinta/logging.hpp>

#include <stdint.h> // uint32_t
#include <string>   // std::string
#include <vector>   // std::vector

namespace uinta {
namespace resource {

inline const uint16_t UINTA_RES_MAX_RESOURCES      = 256;
inline const uint16_t UINTA_RES_HANDLE_T_MAX_VALUE = 65535;
inline const uint16_t UINTA_RES_ID_MASK    = 0xFFFF;
inline const uint32_t UINTA_RES_VALID_MASK = 0x10000;
inline const u_char UINTA_RES_VALID_SHIFT = 16;

using resource_handle_t = uint32_t;

struct ResourceHandle {
  friend class ResourceManager;

  ResourceHandle() : ResourceHandle(0) {}
  ResourceHandle(const resource_handle_t handle) : handle(handle) {}
  ResourceHandle(const ResourceHandle &other) { *this = other; }

  ResourceHandle &operator=(const ResourceHandle &other) {
    handle = other.handle;
    return *this;
  }

  resource_handle_t getId() const { return handle & UINTA_RES_ID_MASK; }
  bool isValid() const { return handle & UINTA_RES_VALID_MASK; }

private:
  resource_handle_t handle : 32;
};

class ResourceManager {
public:
  ResourceManager();

  ResourceHandle *const registerResource(const std::string &relativePath);

  void loadResource(const ResourceHandle *handle);
  uint32_t getSize(const ResourceHandle *handle);

private:
  std::vector<ResourceHandle> handles;
  std::vector<std::string> paths;
  uint32_t sizes[UINTA_RES_MAX_RESOURCES];

  logger_t logger;

  ResourceHandle *const generateHandle(const std::string &relativePath);

  ResourceHandle *const findHandle(const std::string &relativePath);

  void setId(ResourceHandle &handle, const resource_handle_t id);

  void invalidate(ResourceHandle &handle) { handle.handle &= ~(1UL << UINTA_RES_VALID_SHIFT); }
  void validate(ResourceHandle &handle) { handle.handle |= 1UL << UINTA_RES_VALID_SHIFT; }
};

namespace internal {
void parsePaths(const char *const paths, std::vector<std::string> &buffer, const logger_t &logger);
} // namespace internal

} // namespace resource
} // namespace uinta

#endif // UINTA_RESOURCE_HPP
