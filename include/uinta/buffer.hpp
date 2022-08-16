#ifndef UINTA_BUFFER_HPP
#define UINTA_BUFFER_HPP

#include <cstdint>

namespace uinta {

struct GpuMemoryArena {
  uint32_t vboId;
  unsigned int count;
  unsigned int max;
  unsigned int offset;

  GpuMemoryArena() noexcept {
    vboId = 0;
    count = 0;
    max = 0;
    offset = 0;
  }

  GpuMemoryArena(const GpuMemoryArena& other) noexcept { *this = other; }

  GpuMemoryArena& operator=(const GpuMemoryArena& other) noexcept {
    vboId = other.vboId;
    max = other.max;
    count = other.count;
    offset = other.offset;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_BUFFER_HPP
