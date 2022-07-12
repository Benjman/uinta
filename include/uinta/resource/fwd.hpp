#ifndef UINTA_RESOURCE_FWD_HPP
#define UINTA_RESOURCE_FWD_HPP

#include <cstdint>

#include "../macros.hpp"

namespace uinta {

using resource_t = uint32_t;
using resource_size_t = uint32_t;

inline const std::size_t UINTA_RES_MANAGER_STORAGE_SIZE = MEGABYTES(10);

inline const resource_t UINTA_RES_ID_MASK = 0xFFFF;
inline const resource_t UINTA_RES_TYPE_MASK = 0x70000;
inline const resource_t UINTA_RES_BUFFERED_MASK = 0x80000;
inline const resource_t UINTA_RES_ACTIVE_MASK = 0x100000;

// Resource types are limited to 8 unique values:
enum ResourceType : resource_t {
  Binary = 0x00000,
  Text = 0x10000,
  // UNUSED2 = 0x20000,
  // UNUSED3 = 0x30000,
  // UNUSED4 = 0x40000,
  // UNUSED5 = 0x50000,
  // UNUSED6 = 0x60000,
  // UNUSED7 = 0x70000,
};

}  // namespace uinta

#endif  // UINTA_RESOURCE_FWD_HPP
