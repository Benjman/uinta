#ifndef UINTA_IO_FWD_HPP
#define UINTA_IO_FWD_HPP

#include <cstdint>

#include "../macros.hpp"

namespace uinta {

using file_t = uint32_t;
using file_size_t = uint32_t;

inline const std::size_t UINTA_FILE_MANAGER_STORAGE_SIZE = MEGABYTES(10);

inline const file_t UINTA_FILE_ID_MASK = 0xFFFF;
inline const file_t UINTA_FILE_TYPE_MASK = 0x70000;
inline const file_t UINTA_FILE_IS_BUFFERED_MASK = 0x80000;
inline const file_t UINTA_FILE_IS_ACTIVE_MASK = 0x100000;

// File types are limited to 8 unique values:
enum FileType : file_t {
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

#endif  // UINTA_IO_FWD_HPP
